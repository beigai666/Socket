#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ws2tcpip.h>
#include <WinSock2.h>
class Net
{
protected:
	int sock;
public:
	SOCKADDR_IN servAddr;
	SOCKADDR_IN clieAddr;
	Net();

};

