#ifndef __SOCK_LIB__
#define __SOCK_LIB__

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <ctype.h>

typedef int     sfd_t;              /* socket type */
typedef struct  sockaddr_in addr_t; /* address type */

/********************General Purppose**************************/
sfd_t sock_tcp(void);
sfd_t sock_udp(void);
addr_t sock_addr(const char* ip, int port);
int sock_reuse(sfd_t s);
int sock_shut(sfd_t s);

/*************************Unicast******************************/
int sock_send(sfd_t s, uint8_t* bytes, int size);
int sock_receive(sfd_t s, uint8_t* bytes, int size);

/*Passive*/
int sock_bind(sfd_t s, addr_t addr);
int sock_listen(sfd_t s, int len);
sfd_t sock_accept(sfd_t s, addr_t addr);

/*Active*/
sfd_t sock_connect(sfd_t s, addr_t addr);

/************************Multicast*****************************/
int sock_multicast(sfd_t s, const char* ipgroup);
int sock_receivefrom(sfd_t s, uint8_t* buffer, int size, addr_t addr);
int sock_sendto(sfd_t s, uint8_t* bytes, int size, addr_t addr);


#endif /*__SOCK_LIB__*/