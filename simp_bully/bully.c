/*******************************************************************************
 * Simplfied Bully election                                                    *
 * Multicasting Election Message allows to eliminate BACKOFF and HALT messages *
 *******************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <time.h>
#include <sys/time.h>

#define MSGLEN 10

enum bullyState{
    START,
    ELECTION,
    LEADER,
    STALEMATE,
    NOTCANDIDATE
};

void add_time(struct timezone *tz, struct timeval *tv, uint ms){

    tv->tv_usec = tv->tv_usec + ms*1000;
    if(tv->tv_usec >= 1000000){
        tv->tv_usec=0;
        tv->tv_sec++;
    }
    if(tv->tv_sec>=60){
        tv->tv_sec=0;
        tz->tz_minuteswest++;
    }
}

int reachedGoal(struct timezone *tz, struct timeval *tv, struct timezone *goalz, struct timeval *goalv){
    if(tz->tz_minuteswest > goalz->tz_minuteswest)
        return 1;
    if(tz->tz_minuteswest < goalz->tz_minuteswest)
        return 0;
    if(tv->tv_sec > goalv->tv_sec)
        return 1;
    if(tv->tv_sec < goalv->tv_sec)
        return 0;
    if(tv->tv_usec >= goalv->tv_usec)
        return 1;
    else return 0;
}

int main(int argc, char* argv[]){

    char ip[16] = "225.0.0.37";
    int port = 4321;
    uint8_t id = 0;
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
    inet_pton(AF_INET,ip,&addr.sin_addr);

    int optval = 1;
    setsockopt(recvSock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
    bind(recvSock,(struct sockaddr*)&addr,sizeof(addr));
    
    struct ip_mreq mreq;
    inet_aton(ip,&mreq.imr_multiaddr);
    mreq.imr_interface.s_addr = INADDR_ANY;
    int resMcast = setsockopt(recvSock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
    assert(resMcast>=0);

    int flags = fcntl(recvSock,F_GETFL);
    fcntl(recvSock,F_SETFL,flags|O_NONBLOCK);

    /*bidirectional(with itself)*/
/*
    
    int x = 20;
    sendto(sendSock,&x,sizeof(x),0,&addr,sizeof(addr));
    struct sockaddr_in addrin;
    int in;
    socklen_t lenin = sizeof(addrin);
    while(1){
        int re = recvfrom(recvSock,&in,sizeof(in),0,&addrin,&lenin);
        if(re==sizeof(in)){
            printf("RECEIVED: %d\n",in);
            x++;
            printf("SENDING: %d\n\n",x);
            sendto(sendSock,&x,sizeof(x),0,&addr,sizeof(addr));
        }
        sleep(1);
    }
*/
    
    /*BULLY*/
    enum bullyState state = 0;
    char election[MSGLEN]={0};
    sprintf(election,"E%02x",id);
    char leaderm[MSGLEN] = {0};
    sprintf(leaderm,"L%02x",id);

    char buffer[MSGLEN];
    int rbytes = 0;
    struct sockaddr_in addrin;
    addrin.sin_family=AF_INET;
    addrin.sin_port=htons(port);
    inet_pton(AF_INET,ip,&addrin.sin_addr);
    socklen_t slen = sizeof(addrin);
    int leaderid = id;
    
    struct timezone tz,tzgoal;
    struct timeval tv,tvgoal;
    int goal = 0;

    while(1){
        switch(state){
        
        case START: //presents itself and starts counting for 2T
            usleep(delay*1000);
            sendto(sendSock,election,sizeof(election),0,(struct sockaddr*)&addr,sizeof(addr));
            printf("Sent: %s\n",election);
            state = ELECTION;
            gettimeofday(&tvgoal,&tzgoal);
            add_time(&tzgoal,&tvgoal,2*T);

        case ELECTION: //check if new leader with lower id arrived
            rbytes = recvfrom(recvSock,buffer,sizeof(election),0,(struct sockaddr*) &addr,&slen);
            gettimeofday(&tv,&tz);
            goal = reachedGoal(&tz,&tv,&tzgoal,&tvgoal);

            if(rbytes == sizeof(election)){
                sscanf(buffer,"E%02x",&leaderid);
                printf("Received: %s\n",buffer);
            }
            if(leaderid<id){
                state = NOTCANDIDATE;
            }
            else if(leaderid > id){
                state = START;
            }
            if(goal){
                state=LEADER;
            }

            break;
        
        case NOTCANDIDATE://wait to receive leader message
            rbytes = recvfrom(recvSock,buffer,sizeof(election),0,(struct sockaddr*) &addr,&slen);

            if(rbytes == sizeof(leaderm) && buffer[0]==leaderm[0]){
                state = STALEMATE;
                sscanf(buffer,"L%02x",&leaderid);
                printf("Leader is now: %d\n",leaderid);
            }
            break;

        case LEADER: //brags about becoming leader
            sendto(sendSock,leaderm,sizeof(leaderm),0,(struct sockaddr*)&addr,sizeof(addr));
            printf("I AM THE LEADER, SENT: %s\n",leaderm);
            state = STALEMATE;
            break;
        
        case STALEMATE: //waits for new election start, if the new candidate is more powerful, skips to NOTCANDIDATE
            rbytes = recvfrom(recvSock,buffer,sizeof(election),0,(struct sockaddr*) &addr,&slen);

            if(rbytes==sizeof(election) && buffer[0]==election[0]){
                sscanf(buffer,"E%02x",&leaderid);
                state = (id<leaderid) ? START : NOTCANDIDATE;
            }
            break;

        }

    }


    return 0;
}
