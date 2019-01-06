#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdio.h>

int main(){

    int s = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(4321);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);

    connect(s,(struct sockaddr*)&addr,sizeof(addr));

    while(1){
        int data;
        scanf("%d",&data);
        write(s,&data,sizeof(int));
    }
    close(s);
    return 0;
}