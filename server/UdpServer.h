#pragma once
#include "UdpClass.h"
#include <thread>
class UdpServer :
    public UdpClass
{
    int tg;
    std::thread* th;
public:
    UdpServer();
    void run();
    void start();
};

