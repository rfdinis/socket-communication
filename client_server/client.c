#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

void error(const char* err){
    printf("%s\n",err);
    exit(1);
}

int main(){

    int s = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(4321);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);

    if( connect(s,(struct sockaddr*)&addr,sizeof(addr)))
        error("Couldn't Connect.");

    while(1){
        int data;
        scanf("%d",&data);
        write(s,&data,sizeof(int));
    }
    close(s);
    return 0;
}
