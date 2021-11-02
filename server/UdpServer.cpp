#include "UdpServer.h"
#include "NetTransPackage.h"
#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 100
UdpServer::UdpServer() {
    /* socket creation */
    tg = 0;
    int broadcast = 1;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        
        exit(1);
    }
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof broadcast) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        exit(1);
    }
    /* bind local server port */
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(LOCAL_SERVER_PORT);
    int  rc = bind(sock, (struct sockaddr*)&servAddr, sizeof(servAddr));
    if (rc < 0) {
        printf("cannot bind port number %d \n",LOCAL_SERVER_PORT);
        exit(1);
    }

    printf("waiting for data on port UDP %u\n", LOCAL_SERVER_PORT);
    InitializeCriticalSection(&Mutex);
    InitializeConditionVariable(&g_emptyCond);
    //struct timeval tv_out;
    //tv_out.tv_sec = 10;//µÈ´ý10Ãë
    //tv_out.tv_usec = 0;
    //setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof(tv_out));
}



void UdpServer::run() {
    while (true)
    {
        NetTask* task = get_task();
        printf("get task=%p\n", task);

        const char* str = "hello";
        int leng = strlen(str) + 1;
        //NetTransPackage* package = createNetTransPackage(leng, (uint8_t*)str);
        //printf("write start:\n");
        udp_write((char*)str, 5, task->addr);
        //printf("write end:\n");

        //deletePackage(package);

        free(task);
    }

}

int UdpServer::add_task(NetTask* task) {
    EnterCriticalSection(&Mutex);
    if (tasks.size() == 0) {
        WakeConditionVariable(&g_emptyCond);
    }
    tasks.insert(std::pair<void*, NetTask*>(task,task));
    
    LeaveCriticalSection(&Mutex);
    return 0;
}

NetTask* UdpServer::get_task() {
    NetTask* task = NULL;
    EnterCriticalSection(&Mutex);
    if (tasks.size() == 0) {
        SleepConditionVariableCS(&g_emptyCond, &Mutex, INFINITE);
    }
    auto i = tasks.begin();
    task = i->second;
    tasks.erase(i);
    LeaveCriticalSection(&Mutex);
    return task;
}


void UdpServer::start() {
    th = new std::thread(&UdpServer::run, this);
    //th->join();
    void* buf = malloc(1500);
    int num = 0;
    while (1) {
        

        memset(buf, 0,1500);
        printf("read start \n");
        int nNetTimeout = 2000;
        if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int)))
        {
            printf("Set Ser_RecTIMEO error !\r\n");
        }
        int size =udp_read((char*)buf, 1500);
        if (size == -1) {
            continue;
        }
        printf("read end \n");
        //NetTransPackage * package =(NetTransPackage*)buf;
        /* print received message */
        printf("num=%d server: from % s : UDP % u : % s \n",num,inet_ntoa(clieAddr.sin_addr),ntohs(clieAddr.sin_port), buf);
        //printf("verification=%d\n", verification(package));
        NetTask* task = (NetTask*)malloc(sizeof(NetTask));
            memset(task, 0, sizeof(NetTask));
            task->addr = clieAddr;
        add_task(task);
      
        tg++;
        num++;
        //read();
    }
    free(buf);
    buf = NULL;
}


