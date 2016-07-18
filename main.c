#include "tiny/tinyweb.h"
#include "pub.h"
#include "defs.h"

int main(int argc ,char **argv)
{
    int listenfd, connfd, port, clientlen;
   	struct sockaddr_in clientaddr;
   	
   	if(argc != 2)
   	{
		fprintf(stderr, "usage: [%s] port\n", argv[0]);
		exit(1);
	}
	
	port = atoi(argv[1]);
	
	listenfd = open_listenfd(port);
	while(1)
	{
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
		if(connfd > 0)
		{
			printf("connect is coming,addr %s\n",inet_ntoa(clientaddr.sin_addr));
		    doit(connfd);
	    }
		close(connfd);
	}
}
