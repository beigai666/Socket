#include "UdpClass.h"
#include "net.h"


UdpClass::UdpClass()
{
}

UdpClass::~UdpClass()
{
}


int UdpClass::udp_write(const char* buf, int leng, SOCKADDR_IN &addr) {
    printf("server: from % s : UDP % u\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    int ret = ::sendto(sock, buf, leng, 0, (struct sockaddr*)&addr, sizeof(addr));

    return ret;
}

int UdpClass::udp_read(char* data, int leng) {
    memset(&clieAddr, 0, sizeof(clieAddr));
    int vaddr_leng = sizeof(clieAddr);
    int ret = ::recvfrom(sock, data, leng, 0, (struct sockaddr*)&clieAddr, &vaddr_leng);
    return ret;
}