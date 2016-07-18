#include "pub.h"
#include "defs.h"

#define LISTENQ 1024
#define MAXEPOLLSIZE 10000
#define NTHREADS 6
#define SBUFSIZE 4
#define SBUF_ITEM_SIZE (sizeof(int))

void *threadfunc(void* vargp);

sbuf_t sbuf;

/*int make_socket_non_blocking(int fd) {
    int flags, s;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
       // log_err("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl(fd, F_SETFL, flags);
    if (s == -1) {
        //log_err("fcntl");
        return -1;
    }

    return 0;
}
*/ 
int main(int argc ,char **argv)
{
    int listenfd, connfd, port, clientlen, i;
    
    int listener, new_fd, nfds, n, ret, new; 
    struct epoll_event ev; 
    int kdpfd, curfds; 
    socklen_t len; 
    struct sockaddr_in my_addr, their_addr; 
    unsigned int myport, lisnum; 
    struct epoll_event events[MAXEPOLLSIZE]; 
    struct rlimit rt;
    
   	struct sockaddr_in clientaddr;
   	pthread_t tid[NTHREADS];
   	
   	if(argc != 2)
   	{
		fprintf(stderr, "usage: [%s] port\n", argv[0]);
		exit(1);
	}
	
	port = atoi(argv[1]);
	
	//sbuf_init(&sbuf, SBUFSIZE, SBUF_ITEM_SIZE);
	
	struct threadpool *pool = threadpool_init(10, 20);
	
	listener = open_listenfd(port);
	kdpfd = open_epoll_create(listener);
	len = sizeof(struct sockaddr_in); 
	curfds = 1; 
    while (1) { 
       //printf("OK \n"); 
        /* 等待有事件发生 */ 
        nfds = epoll_wait(kdpfd, events, curfds, -1); 
        if (nfds == -1) { 
            perror("epoll_wait"); 
            continue; 
        } 
        printf(" nfds = %d \n", nfds); 
        /* 处理所有事件 */ 
        for (n = 0; n < nfds; ++n) { 
            if (events[n].data.fd == listener) { 
            while(1){
            	 new_fd = accept(listener, (struct sockaddr *) &their_addr,  &len); 
                     if (new_fd < 0) {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                            /* we have processed all incoming connections */
                            break;
                        } else {
                            printf("err_accept\n");
                            break;
                        }
                    }
					else
					{
                       printf("new_fd =  %d ",new_fd);
					   printf("connect comefrome:%s %d,socket:%d\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_fd);
					}
					                
                     //int r = make_socket_non_blocking(new_fd);
				     new = serve(new_fd); 
				     //设置为非阻塞 
				     //setnonblocking(new); 
				     
                     ev.events = EPOLLIN | EPOLLONESHOT | EPOLLET ; 
                     ev.data.fd = new; 
                     if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, new, &ev) < 0) { 
                         fprintf(stderr, "把 socket '%d' 加入 epoll 失败！%s/n", 
                                 new, strerror(errno)); 
                            return -1; 
                        } 
                     
					 curfds++; 
                     printf("epoll_ctl OK \n");                  
			         }
                                      
            } else{
            	if ((events[i].events & EPOLLERR) ||
                    (events[i].events & EPOLLHUP) ||
                    (!(events[i].events & EPOLLIN))) {
                    //log_err("epoll error fd: %d", r->fd);
                    close(events[n].data.fd);
                    curfds--; 
                    continue;
                }
            	
            	threadpool_add_job(pool, threadfunc, (void*)&new);
            	
			}
        }
    }
    close(listener); 
    return 0; 
    
}
  void *threadfunc(void* vargp)
  {
  	int connfd;
  	connfd = *(int*)vargp;
  	pthread_detach(pthread_self());
  	if(connfd > 0)
  	{
  		doit(connfd);
  		close(connfd);
	  }
	  else
	  {
	  	printf("fd[%d] is wrong!", connfd);
	  }
  }

