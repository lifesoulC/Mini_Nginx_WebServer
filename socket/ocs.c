#include "ocs.h"
#define MAXEPOLLSIZE 10000

#define ANET_ERR -1
#define ANET_OK 1
/****************************
*****************************/ 
int setnonblocking(int sockfd) //����Ϊ������ 
{ 
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) { 
        return -1; 
    } 
    return 0; 
}
static int count111 = 0;
static time_t oldtime = 0, nowtime = 0;

int make_socket_non_blocking(int fd) {
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


/***********************************************************************
 * 
 * ********************************************************************/
int open_clientfd(char *hostname, int port)
{
	printf("Client Startup, ready connect to [%s:%d].\n",hostname,port);
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;
    
    if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
		printf("Client create socket Fail!\n");
        return -1;
    }
    if((hp = gethostbyname(hostname)) == NULL)
    {
		printf("Get host by hostname[%s] fail!\n",hostname);
        return -2;
    }    
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    memcpy((char *)&serveraddr.sin_addr.s_addr,
        (char *)&hp->h_addr_list[0], hp->h_length);
    serveraddr.sin_port = htons(port);
    
    if(connect(clientfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
    {
		printf("Connect to [%s] Fail!\n",inet_ntoa(serveraddr.sin_addr));
		return -1;
    }
    return clientfd;
    
}
/***********************************************************************
 * 
 * ********************************************************************/
 /*
int open_clientfdbyaddr(char *hostname, int port)
{
	printf("Client ready connect to [%s:%d].\n",hostname,port);
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;
    
    if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
		printf("Client create socket Fail!\n");
        return -1;
    }
    if((hp = gethostbyname(hostname)) == NULL)
    {
		printf("Get host by hostname[%s] fail!\n",hostname);
        return -2;
    }    
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    memcpy((char *)&serveraddr.sin_addr.s_addr,
        (char *)&hp->h_addr_list[0], hp->h_length);
    serveraddr.sin_port = htons(port);
    
    if(connect(clientfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;
    
    return clientfd;
    
}
*/
/***********************************************************************
 * 
 * ********************************************************************/
int open_listenfd(int port)
{
		
    int listener, new_fd, nfds, n, ret; 
    struct epoll_event ev; 
    int kdpfd, curfds; 
    socklen_t len; 
    struct sockaddr_in my_addr, their_addr; 
    unsigned int myport, lisnum; 
    struct epoll_event events[MAXEPOLLSIZE]; 
    struct rlimit rt;


    /* ����ÿ����������򿪵�����ļ��� */ 
    rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE; 
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1) { 
        perror("setrlimit"); 
        exit(1); 
    } else 
	printf("setrlimit ok/n");

    /* ���� socket ���� */ 
    if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1) { 
        perror("socket"); 
        exit(1); 
    } else 
        printf("socket creat OK/n");

    /*����socket���ԣ��˿ڿ�������*/ 
   ///int opt=SO_REUSEADDR; 
   //setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

   /* �ڵ���closesocket��ʱ�򲻻����̷��أ��ں˻��ӳ�һ��ʱ�䣬���ʱ�����l_linger��ֵ�������������ʱʱ�䵽��֮ǰ��������δ���͵�����(����FIN��)���õ���һ�˵�ȷ�ϣ�closesocket�᷵����ȷ��socket�������������˳�������closesocket��ֱ�ӷ��ش���ֵ��δ�������ݶ�ʧ��socket��������ǿ�����˳�����Ҫע���ʱ�����socket������������Ϊ�Ƕ����ͣ���closesocket��ֱ�ӷ���ֵ��*/
   struct linger linger;
   linger.l_onoff  =1;
   linger.l_linger =1;
   if (setsockopt(listener,SOL_SOCKET,SO_LINGER,&linger,sizeof(linger)) == -1) {
	exit(1);
    }


    /*����socketΪ������ģʽ*/ 
    setnonblocking(listener);
    
    bzero(&my_addr, sizeof(my_addr)); 
    my_addr.sin_family = PF_INET; 
    my_addr.sin_port = htons(port); 
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind 
        (listener, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) 
        == -1) { 
            perror("bind"); 
            exit(1); 
    } else 
        printf("IP bind OK/n");

    if (listen(listener,LISTENQ) == -1) { 
        perror("listen"); 
        exit(1); 
    } else 
        printf("listen OK/n");

    /* ���� epoll ������Ѽ��� socket ���뵽 epoll ������ */ 
    return listener;
    
}
int open_epoll_create(int listener)
{
  
    struct epoll_event ev; 
    int kdpfd, curfds; 
    socklen_t len; 
    struct sockaddr_in my_addr, their_addr; 
    unsigned int myport, lisnum; 
    struct epoll_event events[MAXEPOLLSIZE]; 
    
    kdpfd = epoll_create(MAXEPOLLSIZE); 
    len = sizeof(struct sockaddr_in); 
    ev.events = EPOLLIN | EPOLLET; 
    ev.data.fd = listener; 
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listener, &ev) < 0) { 
        fprintf(stderr, "epoll set insertion error: fd=%d/n", listener); 
        return -1; 
    } else 
        printf("listen socket add epoll OK/n"); 
   
	
 return kdpfd;

}



/* Set TCP keep alive option to detect dead peers. The interval option
 * is only used for Linux as we are using Linux-specific APIs to set
 * the probe send time, interval, and count. */
int anetKeepAlive(int fd, int interval)
{
    int val = 1;
	//����keepalive����
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) == -1)
    {
       // anetSetError(err, "setsockopt SO_KEEPALIVE: %s", strerror(errno));
        return ANET_ERR;
    }

#ifdef __linux__
    /* Default settings are more or less garbage, with the keepalive time
     * set to 7200 by default on Linux. Modify settings to make the feature
     * actually useful. */

    /* Send first probe after interval. */
    val = interval;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &val, sizeof(val)) < 0) {
       // anetSetError(err, "setsockopt TCP_KEEPIDLE: %s\n", strerror(errno));
        return ANET_ERR;
    }

    /* Send next probes after the specified interval. Note that we set the
     * delay as interval / 3, as we send three probes before detecting
     * an error (see the next setsockopt call). */
    val = interval/3;
    if (val == 0) val = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &val, sizeof(val)) < 0) {
       // anetSetError(err, "setsockopt TCP_KEEPINTVL: %s\n", strerror(errno));
        return ANET_ERR;
    }

    /* Consider the socket in error state after three we send three ACK
     * probes without getting a reply. */
    val = 3;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &val, sizeof(val)) < 0) {
        //anetSetError(err, "setsockopt TCP_KEEPCNT: %s\n", strerror(errno));
        return ANET_ERR;
    }
#endif

    return ANET_OK;
}

int serve(int new_fd)
{
   /*����socket���ԣ��˿ڿ�������*/ 
   int opt=SO_REUSEADDR; 
   setsockopt(new_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

   /* �ڵ���closesocket��ʱ�򲻻����̷��أ��ں˻��ӳ�һ��ʱ�䣬���ʱ�����l_linger��ֵ��������
   �����ʱʱ�䵽��֮ǰ��������δ���͵�����(����FIN��)���õ���һ��   ��ȷ�ϣ�closesocket�᷵����ȷ��
   socket�������������˳�������closesocket��ֱ�ӷ��ش���ֵ��δ�������ݶ�ʧ��socket��������ǿ�����˳���
   ��Ҫע���ʱ�����s  ocket������������Ϊ�Ƕ����ͣ���closesocket��ֱ�ӷ���ֵ��*/ 
   struct linger linger;
   linger.l_onoff  =1;
   linger.l_linger =1;
   if (setsockopt(new_fd,SOL_SOCKET,SO_LINGER,&linger,sizeof(linger)) == -1) {
	exit(1);
    }


    /*����socketΪ������ģʽ*/ 
    setnonblocking(new_fd);
    
     //������ 
    int g = anetKeepAlive(new_fd, 10);

    int optval = 1;
    if(setsockopt(new_fd,SOL_SOCKET,SO_REUSEADDR,
       (const void*)&optval,sizeof(int)) < 0)
     {
	   printf("clear other server fail \n");
	   return -1;
     }

   return new_fd;
}
 /* ���� 
    int optval;
    size_t optlen;
    optval = 1 ;
    optlen = sizeof(optval);
    if (setsockopt(new_fd,SOL_SOCKET,SO_KEEPALIVE,&optval,optlen)== -1) {
	//perror("KEEPALIVE");
     	//fprintf(stdout,"errno = <%d> unable to setsockopt KEEPALIVE\n",errno);
     	printf("unable to so_keepalive\n");
        exit(1);
      };

    optval = 1;
    optlen = sizeof(optval);
    if (getsockopt(new_fd,SOL_SOCKET,SO_KEEPALIVE,&optval,&optlen)== -1) {
	//perror("KEEPALIVE");
	//printf(stdout,"errno = <%d> setsockopt KEEPALIVE failed\n",errno);
        printf("KEEPALIVE failed\n");
	exit(1);
      };	
*/
