#pragma once
#include "TcpClass.h"
#include <thread>
#include <iostream>
class Client :
    public TcpClass
{
    std::thread* th;
    void read_message();
public:
    Client();
    void start();
    void run();
};

