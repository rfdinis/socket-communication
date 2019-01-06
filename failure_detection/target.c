#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char* argv[]){

    int port = 4321;
    if(argc>=2){
        port = atol(argv[1]);
    }
    int fd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    inet_pton(AF_INET,"0.0.0.0",&addr.sin_addr);

    bind(fd,(struct sockaddr*)&addr,sizeof(addr));
    listen(fd,4);
    
    char buff[20]={0};
    char pingmsg[]  = "ping";
    char pingresp[] = "pong";
    socklen_t len = sizeof(addr);

    while(1){
        int sock = accept(fd,(struct sockaddr*)&addr,&len);
        assert(sock>=0);
        printf("[MONITOR] Accepted connection\n");
        for(int i=0;i<10;i++){
            int ret = read(sock,buff,sizeof(pingmsg));
            printf("[TARGET] Received: %s\n",buff);
            printf("[TARGET] Sleeping for %ds\n",i);
            sleep(i);
            if(strcmp(buff,pingmsg)==0){
                printf("[TARGET] Sending: %s\n",pingresp);
                write(sock,pingresp,sizeof(pingresp));
            }
            else if(ret<=0){
                break;
            }
            buff[0]='\0';
        }
        printf("[TARGET] Closing connection.\n");
        close(sock);
    }

    close(fd);



    return 0;

}