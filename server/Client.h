#pragma once
#include "TcpClass.h"
#include "Server.h"
#include <thread>
class Client : public TcpClass
{
	std::thread* th;
public:
	struct line* list_node;
	void start();
	void run();
	Client(struct sockaddr_in &serv,struct sockaddr_in &clie,int socket);
};

