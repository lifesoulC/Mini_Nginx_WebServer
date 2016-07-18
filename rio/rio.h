#ifndef _RIO_H
#define _RIO_H

#include "../defs.h"

/*******************************************************************
 * 
 * ****************************************************************/
 #define RIO_BUFSIZE 8192
/*******************************************************************
 * 
 * ****************************************************************/
typedef struct tagTrio_t
{
    int rio_fd;
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFSIZE];	
}rio_t;
/******************************************************************
 * 
 * ***************************************************************/

ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
#endif
