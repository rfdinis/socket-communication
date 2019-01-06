/*******************************************************************************
 * Simplfied Bully election                                                    *
 * Multicasting Election Message allows to eliminate BACKOFF and HALT messages *
 *******************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum bullyState{
    NOLEADER,
    ELECTION,
    LEADER,
    NOTLEADER
};

int main(int argc, char* argv[]){

    char ip[16] = "225.0.0.37";
    int port = 4321;
    int id = 0;
    int T = 100;     /*milliseconds*/
    int delay = 500; /*milliseconds*/

    if(argc >= 5){
        strcpy(ip,argv[1]);
        port = atol(argv[2]);
        T = atol(argv[3]);
        delay = atol(argv[4]);
    }
    else{ printf("Could use: ./bully <ip> <port> <id> <T_ms> <delay_ms>"); }

    printf("[INIT BULLY]\n");
    printf("Multicast IP: %s:%d\n",ip,port);
    printf("ID: %d\n",id);
    printf("T: %d\n",T);
    printf("Delay: %d\n",delay);

    int recvSock = socket(PF_INET,SOCK_DGRAM,0);
    int sendSock = socket(PF_INET,SOCK_DGRAM,0);

    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    inet_pton(AF_INET,"0.0.0.0",&addr.sin_addr);

    int optval = 1;
    setsockopt(recvSock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
    bind(recvSock,(struct sockaddr*)&addr,sizeof(addr));
    
    struct ip_mreq mreq;
    inet_aton(ip,&mreq.imr_multiaddr);
    mreq.imr_interface.s_addr = INADDR_ANY;
    int resMcast = setsockopt(recvSock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
    assert(resMcast>=0);

    /*BULLY*/

    enum bullyState state = 0;
    while(1){
        switch(state){

        case NOLEADER:

            break;
        case ELECTION:

            break;
        case LEADER:

            break;
        case NOTLEADER:

            break;
        }

    }


    return 0;
}
