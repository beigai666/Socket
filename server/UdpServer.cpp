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
}

void UdpServer::run() {
    while (1) {
        if (tg > 0) {
            const char* str = "plokmijnuhbygvtfcrdxeszwaq,ol.p;/";
            int leng = strlen(str) + 1;
            NetTransPackage* package = createNetTransPackage(leng, (uint8_t*)str);
            udp_write((char*)package, sizeof(NetTransPackage) + leng);
            deletePackage(package);
        }

        Sleep(1000);
    }
}

void UdpServer::start() {
    th = new std::thread(&UdpServer::run, this);
    //th->join();
    void* buf = malloc(1500);
    int num = 0;
    while (1) {
        memset(buf, 0,1500);
        int size =udp_read((char*)buf, 1500);
        NetTransPackage * package =(NetTransPackage*)buf;
        /* print received message */
        printf("num=%d server: from % s : UDP % u : % s \n",num,inet_ntoa(clieAddr.sin_addr),ntohs(clieAddr.sin_port), package->Data);
        printf("verification=%d\n", verification(package));
        tg++;
        num++;
    }
    free(buf);
    buf = NULL;
}