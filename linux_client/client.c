#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include "message.h"
#include "tcp_client.h"
#include "broadcast.h"
#include "mul.h"
int main()
{
    //mul();
    //broadcast();
#if 1
    int i = 0;
    char* test = "D.T.Software";
    //char* test = "D.T.So";
    Message* pm = NULL;
    TcpClient* client = TcpClient_New();

    if (client && TcpClient_Connect(client, "127.0.0.1", 8888))
    {
        printf("connect success\n");

        for (i = 0; i < strlen(test); i++)
        {
            char buf[2] = { 0 };

            buf[0] = test[i];

            pm = Message_New(128, 129, i, strlen(test), buf, 2);

            TcpClient_SendMsg(client, pm);
            free(pm);
        }
        //char buf[2] = { 'A',0};
        //TcpClient_SendOOB(client, buf);
    }
    getchar();
    TcpClient_Del(client);
    
#endif
    

    return 0;
}