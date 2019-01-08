#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdio.h>

int main(){
    int s = socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(4321);
    inet_pton(AF_INET,"225.0.0.37", (struct sockaddr*)&addr.sin_addr);

    int optval=1;
    setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

    bind(s,(struct sockaddr*)&addr,sizeof(addr));

    struct ip_mreq mreq;
    inet_aton("225.0.0.37", &mreq.imr_multiaddr);
    mreq.imr_interface.s_addr = INADDR_ANY;
    setsockopt(s,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));

    int buff=0;
    socklen_t len = sizeof(addr);
    while(1){
        recvfrom(s,&buff,sizeof(int),0,(struct sockaddr*)&addr,&len);
        printf("%d\n",buff);
    }

    close(s);
    return 0;
}
