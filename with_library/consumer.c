#include "socklib.h"
#include <stdio.h>

#define SIZE 5

#define PORT (4321)
#define ip_group "225.0.0.37"

int main(){
	sfd_t fd = sock_udp();
	sock_reuse(fd);
	addr_t addr = sock_addr("INADDR_ANY",PORT);
	sock_bind(fd,addr);
	sock_multicast(fd,ip_group);
	uint8_t buffer[SIZE];
	while(1){
		printf("Waiting for message...\n");
		sock_receivefrom(fd, buffer, SIZE-1, sock_addr("0:0:0:0",0));
		buffer[SIZE-1]='\0';
		printf("Received: %s\n",buffer);
	}
	sock_shut(fd);
	
	return 0;
}
