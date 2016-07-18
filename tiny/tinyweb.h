#ifndef _TINY_WEB_H
#define _TINY_WEB_H

#include "../defs.h"
#include "../pub.h"


#define MAXLINE 1024
#define MAXBUF  1024*10
#define DYNAMIC_PAGE 0
#define STATIC_PAGE 1

extern char **environ;

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
#endif
