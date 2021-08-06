#include "Client.h"
#include "message.h"
Client::Client(struct sockaddr_in& serv, struct sockaddr_in& clie, int socket) :TcpClass(){

	servAddr = serv;
	clieAddr = clie;
	sock = socket;
}


void Client::start() {
	th = new std::thread(&Client::run,this);
}
void Client::run(){
	char buf[1024];

	while (true)
	{
		Message msg;
		if (tcp_read((char*)&msg, 8) < 0) {
			break;
		}
		Message* temp = getMsg(&msg);
		if (tcp_read(temp->buf, temp->leng)<0) {
			break;
		}
		
		printf("ip: % s, port : % d:message = %s\n", inet_ntoa(clieAddr.sin_addr), ntohs(clieAddr.sin_port),temp->buf);
		tcp_write((char*)temp,temp->leng+8);
		deleteMessage(temp);
	}
}