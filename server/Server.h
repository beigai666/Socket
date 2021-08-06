#pragma once
#include "TcpClass.h"
#include "Client.h"
#include <thread>

class Client;
typedef struct line {
    struct line* prior; //指向直接前趋
    Client *client;
    struct line* next; //指向直接后继
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

