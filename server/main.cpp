#include <ws2tcpip.h>
#include <WinSock2.h>
#include <iostream>
#include <thread>
#include "Server.h"
#include "UdpServer.h"
#pragma comment(lib,"ws2_32.lib")
int main()
{
	//Server* server = NULL;
	WORD sockVersion = MAKEWORD(2, 2);	//����Winsock  �汾2.2
	WSAData wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) {	//������Ӧ��socket���
		std::cout << "WSA��ʼ��ʧ��" << WSAGetLastError() << "\n";
		system("pause");
		return -1;
	}
	//server = new Server();
	//server->start();
	UdpServer* udpServer = new UdpServer;
	udpServer->start();
    std::cout << "Hello World!\n";
}