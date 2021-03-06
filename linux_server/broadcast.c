#include "broadcast.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int broadcast()
{
    int server = 0;
    struct sockaddr_in saddr = { 0 };
    int client = 0;
    struct sockaddr_in remote = { 0 };
    socklen_t asize = 0;
    int len = 0;
    char buf[32] = "D.T.Software";
    int r = 0;
    int brd = 1;

    server = socket(PF_INET, SOCK_DGRAM, 0);

    if (server == -1)
    {
        printf("server socket error\n");
        return -1;
    }

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(8888);
    
    if (bind(server, (struct sockaddr*)&saddr, sizeof(saddr)) == -1)
    {
        printf("udp server bind error\n");
        return -1;
    }

    printf("udp server start success\n");

    remote.sin_family = AF_INET;
    //remote.sin_addr.s_addr = inet_addr("172.16.255.255");
    remote.sin_addr.s_addr = 0xFFFFFFFF;
    remote.sin_port = htons(8001);

    // brd = 0;
    setsockopt(server, SOL_SOCKET, SO_BROADCAST, &brd, sizeof(brd));
    

    while (1)
    {
        len = sizeof(remote);

        r = strlen(buf);

        sendto(server, buf, r, 0, (struct sockaddr*)&remote, len);
        //printf("buf=%s",buf);
        sleep(1);
    }

    close(server);

    return 0;
}
