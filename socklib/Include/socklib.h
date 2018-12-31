#ifndef __SOCK_LIB__
#define __SOCK_LIB__

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <ctype.h>

typedef int     sfd_t;              /* socket type */
typedef struct  sockaddr_in addr_t; /* address type */


#endif /*__SOCK_LIB__*/