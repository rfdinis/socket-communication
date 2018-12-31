#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>

#include "socklib.h"

sfd_t sock_tcp(void){
	sfd_t s = socket(PF_INET,SOCK_STREAM,0);
	assert(s>=0);
	return s;
}

sfd_t sock_udp(void){
	sfd_t s = socket(PF_INET,SOCK_DGRAM,0);
	assert(s>=0);
	return s;
}

addr_t sock_addr(const char* ip, int port){
    addr_t addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET,ip,&addr);
    return addr;
}

int sock_reuse(sfd_t s){
    int what = 1;
    int ret  = setsockopt(s,IPPROTO_IP, IP_ADD_MEMBERSHIP, &what, sizeof(what));
    assert(ret>=0);
    return ret;
}

int sock_multicast(sfd_t s, const char* ipgroup){
    struct ip_mreq mreq;
    
    inet_pton(AF_INET, ipgroup, &mreq.imr_multiaddr.s_addr);
    inet_pton(AF_INET, ipgroup, &mreq.imr_multiaddr.s_addr);

    int ret = setsockopt(s,IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    return ret;
}

int sock_listen(sfd_t s, int len){
    int ret = listen(s,len);
    assert(ret==0);
    return ret;
}

sfd_t sock_connect(sfd_t s, addr_t addr){
    sfd_t s2 = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    assert(s2>=0);
    return s2;
}

int sock_bind(sfd_t s, addr_t addr){
    int ret = bind(s,(struct sockaddr*)&addr, sizeof(addr));
    assert(ret==0);
    return ret;
}
