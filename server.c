#include <stdio.h>
#include "socklib.h"

#define PORT (4321)
#define BUFFER_SIZE (5)

int main(){

    sfd_t fd = sock_tcp();
    sock_reuse(fd);
    sock_bind(fd,sock_addr("localhost",PORT));
    
    sock_listen(fd,5);
    addr_t client;

    printf("waiting for a client...\n");
    sfd_t sock = sock_accept(fd, &client);
    printf("client on port: %d\n",client.sin_port);


    char buffer[BUFFER_SIZE];
    while(1){
        sock_receive(sock,(uint8_t *)buffer,BUFFER_SIZE-1);
        buffer[BUFFER_SIZE-1] = '\0'; /*just in case*/
        printf("Received: %s\n",buffer);
    }

    printf("leaving");
    sock_shut(sock);
    sock_shut(fd);
    return 0;
}
