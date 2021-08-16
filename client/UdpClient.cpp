#include "UdpClient.h"
#include "NetTransPackage.h"
#define REMOTE_SERVER_PORT 1500
void UdpClient::start() {
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(REMOTE_SERVER_PORT);
    struct timeval tv_out;
    tv_out.tv_sec = 2;//µÈ´ý10Ãë
    tv_out.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof(tv_out));
    char* buf =(char*) malloc(1500);
    int num = 0;
    while (true)
    {
        printf("*********************start********************8\n");
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        servAddr.sin_port = htons(REMOTE_SERVER_PORT);
        sprintf(buf,"str=%s,num=%d\n","qazwsxedc",num);
        printf("buf=%s",buf);
        NetTransPackage *package =createNetTransPackage(strlen(buf) +1, (uint8_t*)buf);
        udp_write((char*)package, sizeof(NetTransPackage)+ strlen(buf) + 1);
        deletePackage(package);
        memset(buf,0,1500);
        printf("staer read\n");
        int size = udp_read((char*)buf, 1500);
        printf("read end\n");
        package = (NetTransPackage*)buf;
        printf("verification: % d reviec data : % s  \n", verification(package), package->Data);
        num++;
        printf("*********************end********************8\n");
        Sleep(1000);
    }
}

UdpClient::UdpClient() {
    int broadcast = 1;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        exit(1);
    }
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof broadcast) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        exit(1);
    }
}