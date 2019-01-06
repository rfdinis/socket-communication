#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <assert.h>
#include <stdio.h>

int main(){

    int s = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    int yes=1;
    setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(4321);
    inet_pton(AF_INET,"0.0.0.0",&addr.sin_addr);
    bind(s,(struct sockaddr*)&addr,sizeof(addr));
    listen(s,4);
    while(1){
        socklen_t len=sizeof(addr);
        int s2 = accept(s,(struct sockaddr*)&addr,&len);
        assert(s2>=0);
        while(1){
            int buff=0;
            read(s2,&buff,4);
            printf("%d\n",buff);
            if(buff<=0) break;
        }

    }
    close(s);
    return 0;
}