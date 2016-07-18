
#include "rio.h"

// 
// name: 未知
// @param
// @return
// 
ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;
    
    while(nleft > 0)
    {
		/*Interrupted by sig, call read again*/
	    if((nread = read(fd, bufp, nleft)) <0 )
	    {
			if(errno == EINTR)
			    nread = 0;
			else
			    return -1;
		}
		/*EOF*/
		else if(nread == 0)
	        break;
	    nleft -= nread;
	    bufp += nread;
	}
	return n-nleft;	
}
// 
// name: 未知
// @param
// @return
// 
ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;
    
    while(nleft > 0)
    {
		if((nwritten = write(fd, bufp, nleft)) <= 0)
		{
			if(errno == EINTR)
			    nwritten = 0;
			/* errno set by writel*/
			else
			    return -1;
		}
		nleft -= nwritten;
		bufp += nwritten;
	}	
	return n;
}
// 
// name: 未知
// @param
// @return
//
void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;	
} 
// 
// name: 未知
// @param
// @return
//
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
	
	int cnt;
	/*fill the buf if is empty*/
	while(rp->rio_cnt <= 0)
	{
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
		
		if(rp->rio_cnt < 0)
		{
			if(errno != EINTR)
			{
				return -1;
			}
			/* EOF*/
			else if(rp->rio_cnt == 0)
			    return 0;
			else
			    rp->rio_bufptr = rp->rio_buf;
		 }
	 }
	 
	 cnt = n;
	 if(rp->rio_cnt < n)
	     cnt = rp->rio_cnt;
	 memcpy(usrbuf, rp->rio_bufptr, cnt);
	 rp->rio_bufptr += cnt;
	 rp->rio_cnt -= cnt;
	 return cnt;
}
// 
// name: 未知
// @param
// @return
//
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
	int n, rc;
	char c, *bufp = usrbuf;
    for(n = 1; n < maxlen; n++)
    {
		if((rc = rio_read(rp, &c, 1)) == 1)
		{
			*bufp++ = c;
			/* meet enter*/
			if(c == '\n')
			    break;
		}
		else if(rc == 0)
		{
			/* EOF, no data read*/
		    if(n == 1)
		        return 0;
		    /* EOF, some data was read*/
		    else
		        break;	
		}
		/* Error*/
		else
		    return -1;
	}
	*bufp = 0;
	return n;    	
}
// 
// name: 未知
// @param
// @return
//
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
	size_t nleft = n;
	ssize_t nread;
	char *bufp = usrbuf;
	
	while(nleft > 0)
	{
		if((nread = rio_read(rp, bufp, nleft)) < 0)
		{
			if(errno == EINTR)
			    nread = 0;
			else
			    return -1;
		}
		else if(nread == 0)
		    break;
		nleft -= nread;
		bufp += nread;
	}
    return (n - nleft);
}

