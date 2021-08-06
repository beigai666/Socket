#include "Client.h"
#include "message.h"
#include "myerror.h"
#include<stdio.h>
#include <windows.h>
Client::Client() {
	TcpClass();
	sock = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "socket = " << sock << std::endl;
	if (sock < 0) {
		perror("socket");
		return;
	}
}

void Client::read_message() {
	Message msg;
	tcp_read((char*)&msg,sizeof(msg));
	Message *new_msg = getMsg(&msg);
	tcp_read(new_msg->buf, new_msg->leng);
	printf("readmsg=%s\n",new_msg->buf);
}

void Client::run() {
	
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(2125);

	if (0 > connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)))
	{
		GetFormattedMessage("connect");
		std::cout << "connect Ê§°Ü:" << WSAGetLastError() << "\n";
		std::cout << "connect Ê§°Ü:" << GetLastError() << "\n";
		perror("connect");
		return; //exit //pthread_exit 
	}
	while (true)
	{
		std::cout << "send message" << std::endl;
		const char* buf = "hello word";
		Message *msg =create_message(buf,strlen(buf)+1);
		tcp_write((char*)msg, msg->leng+8);
		std::cout << "read message" << std::endl;
		Message temp;
		
		tcp_read((char*)&temp,8);
		std::cout << "read finsh" << std::endl;
		Message* read =getMsg(&temp);
		tcp_read((char*)read->buf, read->leng);
		printf("message = %s\n", read->buf);
		std::cout << "message =" << read->buf << std::endl;
		Sleep(1000);
		deleteMessage(read);
		deleteMessage(msg);
	}
}

void Client::start() {
	std::cout << "start" << std::endl;
	th = new std::thread(&Client::run, this);
	th->join();
}