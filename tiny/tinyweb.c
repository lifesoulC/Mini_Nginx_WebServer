#include "tinyweb.h"
#include <sys/mman.h>
// 
// name: 未知
// @param
// @return
//
void doit(int fd)
{
	printf("-------------------doit-----------------\n");
    int is_static;
    struct stat sbuf;
    int n;
    char buf[MAXLINE] = {0};
    char method[MAXLINE] = {0};
    char uri[MAXLINE] = {0};
    char version[MAXLINE] = {0};
    char filename[MAXLINE] = {0};
    char cgiargs[MAXLINE] = {0};
    rio_t rio;
    
    rio_readinitb(&rio, fd);
    n = rio_readlineb(&rio, buf, MAXLINE);
    if(n > 0)
    {
	    printf("Read [%d] bytes data from cache\n",n);	
	}
	else
	    printf("No data or Error\n");
    sscanf(buf, "%s %s %s", method, uri, version);
    printf("Method is [%s]\nUri is [%s]\nVersion is [%s]",method,uri,version);
    if(strcasecmp(method, "GET") != 0)
    {
	    clienterror(fd, method, "501", "Not Implemented",
	                 "Tiny does not implement this method");
	    return;	
	}
	/* read msg, and print*/
	read_requesthdrs(&rio);
	
	is_static = parse_uri(uri, filename, cgiargs);
	if(stat(filename, &sbuf) < 0)
	{
		printf("Can't find %s\n",filename);
	    clienterror(fd, filename, "404", "Not found",
	                 "Tiny couldn't find this file");
	    return;	
	}
	
	if(is_static)
	{
		if(!(S_ISREG(sbuf.st_mode))||!(S_IRUSR & sbuf.st_mode))
		{
		    clienterror(fd, filename, "403", "Forbidden",
	                     "Tiny couldn't read the file");
	        return;	
		}
		serve_static(fd, filename, sbuf.st_size);
	}
	else
	{
		if(!(S_ISREG(sbuf.st_mode))||!(S_IXUSR & sbuf.st_mode))
		{
		    clienterror(fd, filename, "403", "Forbidden",
	                     "Tiny couldn't run the CGI program");
	        return;
		}
		serve_dynamic(fd, filename, cgiargs);
	}
}
// 
// name: 未知
// @param
// @return
//
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
	char buf[MAXLINE], body[MAXBUF];
	
	sprintf(body, "<html><title>Tiny Error</title>");
	sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
	sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
	sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
	sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n",body);
	
	sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
	rio_writen(fd ,buf, strlen(buf));
	sprintf(buf, "Content-type: text/html\r\n");
	rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Content-length: %d\r\n\r\n",(int)strlen(body));
	rio_writen(fd, buf, strlen(buf));
	rio_writen(fd, body, strlen(body));
}
// 
// name: 未知
// @param
// @return
//
void read_requesthdrs(rio_t *rp)
{
	char buf[MAXLINE];
	
	rio_readlineb(rp, buf, MAXLINE);
	while(strcmp(buf, "\r\n"))
	{
		rio_readlineb(rp, buf, MAXLINE);
		printf("%s", buf);
	}
	return;
}
// 
// name: 未知
// @param
// @return
//
int parse_uri(char *uri, char *filename, char *cgiargs)
{
	char *ptr;
	
	/*if equals cig-bin, filename is home.html*/
	if(strstr(uri, "cgi-bin") != NULL)
	{
		strcpy(cgiargs, "");
		strcpy(filename, ".");
		strcat(filename, uri);
		printf("%s\n",uri);
		if(strcmp(uri,"/cgi-bin") == 0)
            strcat(filename,"/home.html");
        return STATIC_PAGE;		
	}
	else
	{
		ptr = index(uri, '?');
		if(ptr)
		{
		    strcpy(cgiargs, ptr+1);
		    *ptr = '\0';	
		}
		else
		    strcpy(cgiargs,"");
		strcpy(filename, "dynamic");
		strcat(filename, uri);
		return DYNAMIC_PAGE;
	}
}
// 
// name: 未知
// @param
// @return
//
void serve_dynamic(int fd, char *filename, char *cgiargs)
{
	char buf[MAXLINE], *emptylist[] = { NULL };
	
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Server: Tiny Web Server\r\n");
	rio_writen(fd, buf, strlen(buf));
	
	if(Fork() == 0)
	{
		Setenv("QUERY_STR", cgiargs, 0);
		Dup2(fd, STDOUT_FILENO);
		Execve(filename, emptylist, environ);
	}
	wait(NULL);
}
// 
// name: 未知
// @param
// @return
//
void serve_static(int fd, char *filename, int filesize)
{
	int srcfd;
	char *srcp, filetype[MAXLINE], buf[MAXBUF];
	
	get_filetype(filename, filetype);
	
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
	sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
	sprintf(buf, "%sContent-type:%s\r\n\r\n", buf, filetype);
	rio_writen(fd, buf, strlen(buf));
	
	srcfd = Open(filename, O_RDONLY, 0);
	if(srcfd == -1)
	    return;
	srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
	Close(srcfd);
	rio_writen(fd, srcp, filesize);
	Munmap(srcp, filesize);
}
// 
// name: 未知
// @param
// @return
//
void get_filetype(char *filename, char *filetype)
{
	if(strstr(filename,"html"))
	    strcpy(filetype, "text/html");
	else if(strstr(filename, ".gif"))
	    strcpy(filetype, "image/gif");
	else if(strstr(filename, ".jpg"))
	    strcpy(filetype, "image/jpeg");
	else
	    strcpy(filetype, "text/plain");
}


