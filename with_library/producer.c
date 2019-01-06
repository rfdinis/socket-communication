#include "socklib.h"
#include <stdio.h>
#include <string.h>

#define PORT (4321)
#define ip_group "225.0.0.37"

int main(){

	sfd_t fd = sock_udp();
	uint8_t bytes[50];
	while(1){
		printf("Write message: ");
		scanf("%s",bytes);
		sock_sendto(fd, bytes, strlen((char*)bytes), sock_addr(ip_group,PORT));
		printf("Sent message: %s\n",bytes);
	}
	sock_shut(fd);
	return 0;
}
