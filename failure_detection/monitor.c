#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

int main(int argc, char* argv[]){

    /*default*/
    char ip[16] = "127.0.0.1"; /*16 - max str len*/
    int port = 4321;
    int period = 4; /*seconds*/
    int timeout = 2; /*seconds*/

    if(argc>=5){
        strcpy(ip,argv[1]);
        port = atol(argv[2]);
        period = atol(argv[3]);
        timeout = atol(argv[4]);
    }

    struct timeval stimeout={0};
    stimeout.tv_sec=timeout;
    stimeout.tv_usec=0;
    printf("[INIT MONITOR]\n");
    printf("Destination: %s:%d\n",ip,port);
    printf("Verification Period: %ds\n",period);
    printf("Timeout: %ds\n",timeout);

    int fd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET,ip,&addr.sin_addr);
    
    int ret = setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO, (char* )&stimeout,sizeof(stimeout));
    if(ret<0){
        printf("[MONITOR] Couldn't set timeout\n");
        return -1;
    }

    ret = connect(fd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret<0){
        printf("[MONITOR] Didn't connect to target\n");
        return -1;
    }
    
    char msg[]  = "ping";
    char resp[] = "pong";
    char buff[20]={0};


    while(1){
        printf("[MONITOR] Sending: %s\n",msg);
        write(fd,msg,sizeof(msg));
        ret = read(fd,buff,sizeof(resp));
        printf("[MONITOR] Received: %s\n",buff);
        if(strcmp(buff,resp)!=0){
            printf("[MONITOR] Target crash\n");
            break;
        }
        else{
            printf("[MONITOR] Correct responde, target is alive\n");
        }
        sleep(period); /*incorrect, sif it takes 1s to receive message, then this cycle will be executed every period+1s*/
        buff[0] = '\0';
    }
    printf("[MONITOR] Exiting\n");
    close(fd);

    return 0;
}