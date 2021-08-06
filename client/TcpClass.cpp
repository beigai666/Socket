#include "TcpClass.h"



TcpClass::TcpClass(){
    Net();
}

int TcpClass::tcp_read(char* buf, int leng) {
    int recv_size = 0;
    while (leng > 0)
    {
        int ret = ::recv(sock, buf, leng, 0);
        if (ret < 0) {
            return ret;
        }
        else
        {
            leng -= ret;
        }
        recv_size += ret;
    }
    return recv_size;
}

int TcpClass::tcp_write(char* data, int leng) {
    int send_size = 0;
    while (leng > 0)
    {
        int ret = ::send(sock, data, leng, 0);
        if (ret < 0) {
            return ret;
        }
        else
        {
            leng -= ret;
        }
        send_size += ret;

    }
    return send_size;
}