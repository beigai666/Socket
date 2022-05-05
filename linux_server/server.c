
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include "tcp_server.h"
#include "broadcast.h"
#include "mul.h"
void EventListener(TcpClient* client, int evt)
{
    if (evt == EVT_CONN)
    {
        printf("Connect: %p\n", client);
    }
    else if (evt == EVT_DATA)
    {
        if (TcpClient_Available(client) > 0){
            Message* m = TcpClient_RecvMsg(client);

            if (m)
            {
                char* s = TcpClient_GetData(client);

                if (m->index == 0)
                {
                    s = malloc(m->total + 1);

                    TcpClient_SetData(client, s);
                }

                strcpy(s + m->index, m->payload);

                if ((m->index + 1) == m->total)
                {
                    printf("Data: %s\n", s);

                    free(s);
                }

                free(m);
            }
        }
        
    }
    else if (evt == EVT_CLOSE)
    {
        printf("Close: %p\n", client);
    }
}

int main()
{
    mul();
    broadcast();
#if 0
    TcpServer* server = TcpServer_New();

    if (server)
    {
        int r = TcpServer_Start(server, 8888, 20);

        printf("r = %d\n", r);

        if (r)
        {
            TcpServer_SetListener(server, EventListener);
            TcpServer_DoWork(server);
        }
    }
#endif
    return 0;
}

