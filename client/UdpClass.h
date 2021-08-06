#pragma once
#include"net.h"

class UdpClass : public Net
{
public:
	UdpClass();
	~UdpClass();
	int udp_read(char* buf, int leng);
	int udp_write(const char* data, int leng);

private:

};