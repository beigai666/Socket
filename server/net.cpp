#include "net.h"
Net::Net() {
    memset(&servAddr,0,sizeof(servAddr));
    memset(&clieAddr, 0, sizeof(clieAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(2125);

}

