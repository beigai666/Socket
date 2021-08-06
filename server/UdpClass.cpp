#include "UdpClass.h"
#include "net.h"


UdpClass::UdpClass()
{
}

UdpClass::~UdpClass()
{
}


int UdpClass::udp_write(const char* buf, int leng) {

    int ret = ::sendto(sock, buf, leng, 0, (struct sockaddr*)&clieAddr, sizeof(clieAddr));

    return ret;
}

int UdpClass::udp_read(char* data, int leng) {
    memset(&clieAddr, 0, sizeof(clieAddr));
    int vaddr_leng = sizeof(clieAddr);
    int ret = ::recvfrom(sock, data, leng, 0, (struct sockaddr*)&clieAddr, &vaddr_leng);
    return ret;
}