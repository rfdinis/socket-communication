#include <stdio.h>
#include <string.h>
#include "socklib.h"

#define BUFFER_SIZE (50)
#define PORT (654321UL)

int main(int argc, char* argv[]){

	printf("Hello World");
	sfd_t fd = sock_tcp();
	addr_t addr = sock_addr("127.0.0.1",PORT);
	sfd_t sock = sock_connect(fd, addr);

	char data[BUFFER_SIZE]={0};
	while(strcmp(data,"stop")){
		int len = scanf("%s\n",data);
		printf("sending...\n");
		sock_send(sock, (unsigned char*)data, len);
	}
	sock_shut(fd);
	sock_shut(sock);
	return 0;
}
