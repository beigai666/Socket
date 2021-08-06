#include "UdpClass.h"
#include "net.h"
UdpClass::UdpClass()
{
}

UdpClass::~UdpClass()
{
}


int UdpClass::udp_write(const char* buf, int leng) {
    int ret = ::sendto(sock, buf, leng, 0, (struct sockaddr*)&servAddr, sizeof(servAddr));
    return ret;
}

int UdpClass::udp_read(char* data, int leng) {
    memset(&servAddr, 0, sizeof(servAddr));
	int vaddr_leng = sizeof(servAddr);
    int ret = ::recvfrom(sock, data, leng, 0, (struct sockaddr*)&servAddr, &vaddr_leng);
    return ret;
}