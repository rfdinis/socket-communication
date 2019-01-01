#include <stdio.h>
#include "socklib.h"

#define PORT (654321UL)

int main(){

    sfd_t fd = sock_tcp();
    sock_reuse(fd);
    sock_bind(fd,sock_addr("127.0.0.1",PORT));
    sock_listen(fd,5);
    addr_t client = {0};
    sfd_t sock = sock_accept(fd, &client);
    while(1){
        

    }

    sock_shut(sock);
    sock_shut(fd);
    return 0;
}