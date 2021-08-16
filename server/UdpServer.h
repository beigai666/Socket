#pragma once
#include "UdpClass.h"
#include <thread>
#include <map>
#include "NetTransPackage.h"
typedef struct NetTask {
    int time;
    SOCKADDR_IN addr;
    NetTransPackage package;
    
}NetTask;

class UdpServer :
    public UdpClass
{
    CRITICAL_SECTION Mutex;
    CONDITION_VARIABLE g_emptyCond;
    int tg;
    std::thread* th;
    std::map<void*, NetTask*> tasks;
public:
    int add_task(NetTask* task);
    NetTask* get_task();
    UdpServer();
    void run();
    void start();
};

