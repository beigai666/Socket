#pragma once
#include"net.h"
class TcpClass : public Net
{
public:
	TcpClass();
	int tcp_read(char* buf, int leng);
	int tcp_write(char* data, int leng);
};


