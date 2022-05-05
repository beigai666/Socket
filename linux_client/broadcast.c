#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
int broadcast()
{
    int sock = 0;
    struct sockaddr_in addr = { 0 };
    struct sockaddr_in remote = { 0 };
    int len = 0;
    char buf[128] = { 0 };
    char input[32] = { 0 };
    int r = 0;

    sock = socket(PF_INET, SOCK_DGRAM, 0);

    if (sock == -1)
    {
        printf("socket error\n");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8001);

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        printf("udp bind error\n");
        return -1;
    }

    //remote.sin_family = AF_INET;
    //remote.sin_addr.s_addr = inet_addr("127.0.0.1");
    //remote.sin_port = htons(8888);

    while (1)
    {
        r = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&remote, &len);

        if (r > 0)
        {
            buf[r] = 0;
            time_t rawtime;

            struct tm* ptminfo;

            time(&rawtime);

            ptminfo = localtime(&rawtime);
            printf("ip:%s  ", inet_ntoa(remote.sin_addr));
            printf("current: %02d-%02d-%02d %02d:%02d:%02d  ",

                ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,

                ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
            printf("Receive: %s\n", buf);
        }
        else
        {
            break;
        }
    }

    close(sock);

    return 0;
}