#ifndef _OCS_H
#define _OCS_H

#include "../defs.h"

#include <sys/resource.h>

/*******************************************************************
 * 
 * ****************************************************************/
#define LISTENQ 1024
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <errno.h>
/*******************************************************************
 * 
 * ****************************************************************/
typedef struct sockaddr SA;
/******************************************************************
 * 
 * ***************************************************************/
int open_listenfd(int port);
int open_clientfd(char *hostname, int port);
int open_epoll_create(int listener); 
int setnonblocking(int sockfd); 
int make_socket_non_blocking(int fd); 
#endif
