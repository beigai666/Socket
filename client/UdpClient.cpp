#include "UdpClient.h"
#include "NetTransPackage.h"
#define REMOTE_SERVER_PORT 1500
void UdpClient::start() {
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(REMOTE_SERVER_PORT);
    void* buf = malloc(1500);
    int num = 0;
    while (true)
    {
        const char* str = "qazwsxedcrfvtgbyhnujmik,ol.p;/";
        int leng = strlen(str) + 1;
        NetTransPackage *package =createNetTransPackage(leng, (uint8_t*)str);
        udp_write((char*)package, sizeof(NetTransPackage)+leng);
        deletePackage(package);
        memset(buf,0,1500);
        int size = udp_read((char*)buf, 1500);
        package = (NetTransPackage*)buf;
        printf("num=%d verification:%d\n reviec data:%s  \n",num, verification(package), package->Data);
        Sleep(1000);
        num++;
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