
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <malloc.h>
#include "message.h"
#include "tcp_client.h"
#include "tcp_server.h"
#include <sys/epoll.h>
#define EPOLL
#define FD_SIZE 1024

typedef struct tcp_server
{
    int fd;
#ifdef EPOLL
    int epollfd;
#endif
    int valid;
    Listener cb;
    TcpClient* client[FD_SIZE];
} Server;

TcpServer* TcpServer_New()
{
    Server* ret = malloc(sizeof(Server));
    if (ret)
    {
        int i = 0;
        ret->fd = -1;
        ret->valid = 0;
        ret->cb = NULL;
#ifdef EPOLL
        ret->epollfd = epoll_create(FD_SIZE);
#endif // EPOLL

        for (i = 0; i < FD_SIZE; i++)
        {
            ret->client[i] = NULL;
        }
    }
}

int TcpServer_Start(TcpServer* server, int port, int max)
{
    Server* s = (Server*)server;
    if (s)
    {
        struct sockaddr_in saddr = { 0 };
        s->fd = socket(PF_INET, SOCK_STREAM, 0);
        printf("server fd = %d\n", s->fd);
        s->valid = (s->fd != -1);
        saddr.sin_family = AF_INET;
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
        saddr.sin_port = htons(port);
        s->valid = s->valid && (bind(s->fd, (struct sockaddr*)&saddr, sizeof(saddr)) != -1);
        s->valid = s->valid && (listen(s->fd, max) != -1);
    }
    return s->valid;
}


void TcpServer_Stop(TcpServer* server)
{
    Server* s = (Server*)server;

    if (s)
    {
        int i = 0;

        s->valid = 0;

        close(s->fd);

        for (i = 0; i < FD_SIZE; i++)
        {
            TcpClient_Del(s->client[i]);
            s->client[i] = NULL;
        }
    }
}

void TcpServer_SetListener(TcpServer* server, Listener listener)
{
    Server* s = (Server*)server;

    if (s)
    {
        s->cb = listener;
    }
}

int TcpServer_IsValid(TcpServer* server)
{
    return server ? ((Server*)server)->valid : 0;
}



static int SelectHandler(Server* s, fd_set* rset, fd_set* reads, fd_set* except, int num, int max)
{
    int ret = max;
    int i = 0;
    for (i = 0; i <= max; i++)
    {
        if (FD_ISSET(i, except))
        {
            if (i != s->fd)
            {
                char buf[2] = { 0 };
                TcpClient_RecvOOB(s->client[i], buf);
                printf("oob=%s\n",buf);
            }
        }
        if (FD_ISSET(i, rset))
        {
            int index = i;
            int event = -1;
            if (i == s->fd)
            {
                struct sockaddr_in caddr = { 0 };
                socklen_t asize = sizeof(caddr);
                index = accept(s->fd, &caddr, &asize);
                if (index > -1)
                {
                    FD_SET(index, reads);
                    ret = (index > max) ? index : max;
                    s->client[index] = TcpClient_From(index);
                    event = EVT_CONN;
                }
            }
            else
            {
                event = EVT_DATA;
            }
            if (s->cb)
            {
                if (TcpClient_IsValid(s->client[index]))
                {
                    s->cb(s->client[index], event);
                }
                else
                {
                    if (s->client[index]) {
                        s->cb(s->client[index], EVT_CLOSE);
                    }
                    TcpClient_Del(s->client[index]);
                    s->client[index] = NULL;
                    FD_CLR(index, reads);
                }
            }
        }
    }
} 
#ifdef EPOLL
int EpollHandler(Server* s,struct epoll_event *events,int nfds)
{
    struct epoll_event ev;
    int event = -1;
    for (int i = 0; i < nfds; i++)
    {
        int index = events[i].data.fd;
        if (events[i].data.fd == s->fd) {
            struct sockaddr_in caddr = { 0 };
            socklen_t asize = sizeof(caddr);
            int conn_sock = accept(s->fd,(struct sockaddr*)&caddr, &asize);
            if (conn_sock == -1) {
                perror("accept");
            }
            ev.events = EPOLLIN;
            ev.data.fd = conn_sock;
            if (epoll_ctl(s->epollfd, EPOLL_CTL_ADD, conn_sock,&ev) == -1) {
                perror("epoll_ctl: conn_sock");
            }
            s->client[conn_sock] = TcpClient_From(conn_sock);
            index = conn_sock;
            event = EVT_CONN;
        }
        else
        {
            event = EVT_DATA;
        }
        if (s->cb)
        {
            if (TcpClient_IsValid(s->client[index]))
            {
                s->cb(s->client[index], event);
            }
            else
            {
                if (s->client[index]) {
                    s->cb(s->client[index], EVT_CLOSE);
                }
                if (epoll_ctl(s->epollfd, EPOLL_CTL_DEL, index, &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                }
                TcpClient_Del(s->client[index]);
                s->client[index] = NULL;

            }
        }
    }
}
#endif

void TcpServer_DoWork(TcpServer* server)
{
    Server* s = (Server*)server;
    if (s && s->valid)
    {
#ifdef EPOLL
        struct epoll_event ev; 
        struct epoll_event*  events=malloc(sizeof(struct epoll_event)*10);
        ev.events = EPOLLIN;
        ev.data.fd = s->fd;
        if (epoll_ctl(s->epollfd, EPOLL_CTL_ADD, s->fd, &ev) == -1) {
            perror("epoll_ctl: listen_sock");
        }
        while (s->valid)
        {
            int nfds = epoll_wait(s->epollfd, events, FD_SIZE, -1);
            if (nfds == -1) {
                perror("epoll_pwait");
            }
            EpollHandler(s,events,nfds);
        }
#else
        int max = 0;
        int num = 0;
        fd_set reads = { 0 };
        fd_set rset = { 0 };
        fd_set except = { 0 };
        struct timeval timeout = { 0 };
        FD_ZERO(&reads);
        FD_SET(s->fd, &reads);
        max = s->fd;
        while (s->valid)
        {
            rset = reads;
            except = reads;
            timeout.tv_sec = 0;
            timeout.tv_usec = 10000;
            num = select(max + 1, &rset, NULL, &except, NULL);
            if (num > 0)
            {
                max = SelectHandler(s, &rset, &reads, &except, num, max);
            }
        }
#endif
    }
}

void TcpServer_Del(TcpServer* server)
{
    TcpServer_Stop(server);
    free(server);
}