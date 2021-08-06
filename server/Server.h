#pragma once
#include "TcpClass.h"
#include "Client.h"
#include <thread>

class Client;
typedef struct line {
    struct line* prior; //ָ��ֱ��ǰ��
    Client *client;
    struct line* next; //ָ��ֱ�Ӻ��
}line;

class Server : public TcpClass 
{
    std::thread *th;
public:
    line* clients;
    void addClient(Client* client);
    void deleteClient(Client* client);
    void start();
    void run();
    Server();
    ~Server();
};

