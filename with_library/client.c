#include <stdio.h>
#include <string.h>
#include "socklib.h"

#define BUFFER_SIZE (50)
#define PORT (4321)

int main(int argc, char* argv[]){
	sfd_t fd = sock_tcp();
	addr_t addr = sock_addr("localhost",PORT);
	sock_connect(fd, addr);

	printf("Successfully connected to server...\n");
	char data[BUFFER_SIZE]={0};
	while(strcmp(data,"stop")){
		int len = 0;
		printf("Type message to send: ");
		scanf("%s",data);
		len = strlen(data);
		printf("sending %d byte(s)\n",len);
		sock_send(fd, (unsigned char*)data, len);
	}
	printf("leaving\n");
	sock_shut(fd);
	return 0;
}
