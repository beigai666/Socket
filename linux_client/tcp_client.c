
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include "msg_parser.h"
#include "tcp_client.h"
#include "message.h"
typedef struct tcpClient
{
    int fd;
    MParser* parser;
    void* data;
}Client;

TcpClient* TcpClient_New()
{
    return TcpClient_From(-1);
}

TcpClient* TcpClient_From(int fd)
{
    Client* ret = malloc(sizeof(Client));
    if (ret)
    {
        ret->fd = fd;
        ret->parser = MParser_New();
        ret->data = NULL;
    }
    return ((ret && ret->parser) ? ret : (free(ret), NULL));
}

int TcpClient_SendMsg(TcpClient* client, Message* msg)
{
    int ret = 0;
    Client *c = (Client*)client;
    if (c && msg)
    {
        
        int len = Message_Size(msg);
        char* data = Message_H2N(msg);
        ret = (write(c->fd, data, len) != -1);
        Message_N2H(msg);
    }
    return ret;
}

int TcpClient_SendRaw(TcpClient* client, char* buf, int length)
{
    int ret = 0;
    Client* c = (Client*)client;
    if (c&&buf)
    {
        ret = (send(c->fd, buf, length, 0) != -1);
    }
    return ret;
}

int TcpClient_SendOOB(TcpClient* client, char* buf)
{
    int ret = 0;
    Client* c = (Client*)client;
    if (c && buf)
    {
        ret = (send(c->fd, buf, 1, MSG_OOB) != -1);
    }
    return ret;
}

Message* TcpClient_RecvMsg(TcpClient* client) {
    Message* msg = NULL;
    Client* c = (Client*)client;
    if (c)
    {
        msg = MParser_ReadFd(c->parser, c->fd);
    }
    return msg;
}

int TcpClient_RecvRaw(TcpClient* client, char* buf, int length)
{
    int ret = 0;
    Client* c = (Client*)client;
    if (c)
    {
        ret = recv(c->fd, buf, length, 0);
    }
    return ret;
}

int TcpClient_RecvOOB(TcpClient* client, char* buf)
{
    int ret = 0;
    Client* c = (Client*)client;
    if (c)
    {
        ret = recv(c->fd, buf, 2, MSG_OOB);
    }
    return ret;
}

int TcpClient_Connect(TcpClient* client, char* ip, int port)
{
    int ret = TcpClient_IsValid(client);
    Client* c = (Client*)client;

    if (!ret && ip && ((c->fd = socket(PF_INET, SOCK_STREAM, 0)) != -1))
    {
        struct sockaddr_in addr = { 0 };

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip);
        addr.sin_port = htons(port);

        ret = (connect(c->fd, (struct sockaddr*)&addr, sizeof(addr)) != -1);
    }

    return ret;
}


int TcpClient_IsValid(TcpClient* client)
{
    int ret = 0;
    Client* c = (Client*)client;

    if (c)
    {
        struct tcp_info info = { 0 };
        int l = sizeof(info);

        getsockopt(c->fd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t*)&l);

        ret = (info.tcpi_state == TCP_ESTABLISHED);
    }

    return ret;
}


void TcpClient_Close(TcpClient* client)
{
    Client* c = (Client*)client;

    if (c)
    {
        close(c->fd);

        c->fd = -1;

        MParser_Reset(c->parser);
    }
}



void TcpClient_Del(TcpClient* client)
{
    Client* c = (Client*)client;

    if (c)
    {
        TcpClient_Close(c);
        MParser_Del(c->parser);
        free(c);
    }
}


void TcpClient_SetData(TcpClient* client, void* data)
{
    Client* c = (Client*)client;

    if (c)
    {
        c->data = data;
    }
}

void* TcpClient_GetData(TcpClient* client)
{
    void* ret = NULL;
    Client* c = (Client*)client;

    if (c)
    {
        ret = c->data;
    }

    return ret;
}

int TcpClient_Available(TcpClient* client)
{
    static char c_temp[1024 * 2] = { 0 };
    int ret = -1;
    Client* c = (Client*)client;

    if (c)
    {
        ret = recv(c->fd, c_temp, sizeof(c_temp), MSG_PEEK | MSG_DONTWAIT);
    }

    return ret;
}