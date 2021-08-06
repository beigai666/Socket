#include "Server.h"
#include "Client.h"
#include <iostream>
#include"myerror.h"
void Server::addClient(Client* client) {
	line* temp = (line*)malloc(sizeof(line));
	temp->client = client;
	temp->prior = clients;
	clients->prior->next = temp;

	temp->next = clients->prior;
	clients->prior = temp;
	client->list_node = temp;
}

void Server::deleteClient(Client* client) {
	line* temp = client->list_node;
	temp->next->prior = temp->prior;
	temp->prior->next = temp->next;
	free(temp);
}

void Server::start() {
	th = new std::thread(&Server::run,this);
	th->join();
}

void Server::run() {
	if (listen(sock, 5) == -1) {

	}
	int client_leng = sizeof(clieAddr);
	//接受一个socket链接就创建一个client
	while (true)
	{
		int client_socket = 0;
		client_socket = accept(sock, (struct sockaddr*)&clieAddr, &client_leng);
		printf("client login. ip: %s, port :%d\n", inet_ntoa(clieAddr.sin_addr), ntohs(clieAddr.sin_port));
		if (client_socket < 0) {

		}
		
		Client* client = new Client(servAddr,clieAddr, client_socket);
		addClient(client);
		client->start();
	}
}

Server::Server() {
	
	clients = (line*)malloc(sizeof(line));
	clients->next = clients;
	clients->prior = clients;
	TcpClass();
	sock = socket(AF_INET, SOCK_STREAM,0);
	if (sock<0) {
		GetFormattedMessage("socket");
	}
	std::cout << "sock" << sock <<std::endl;

	if (bind(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
		GetFormattedMessage("bind");
	}
}

Server::~Server() {
	closesocket(sock);
}