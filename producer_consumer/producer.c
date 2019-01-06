#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdio.h>

int main(){

    int s = socket(PF_INET,SOCK_DGRAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(4321);
    inet_pton(AF_INET,"225.0.0.37",&addr.sin_addr);

    int buff = 0;
    while(1){
        scanf("%d",&buff);
        sendto(s,&buff,4,0,(struct sockaddr*)&addr,sizeof(addr));
    }
    close(s);
    return 0;
}