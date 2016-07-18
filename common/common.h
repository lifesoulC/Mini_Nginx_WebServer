#ifndef _COMMON_H
#define _COMMON_H

#include "../defs.h"
#include "../pub.h"

typedef void *(func)(void *);

void Error(char *msg);
pid_t Fork(void);
int Dup2(int oldfd, int newfd);
int Execve(const char *filename, const char *argv[], const char *envp[]);
int Setenv(const char *name, const char *newvalue, int overwrite);
int Open(const char *filename, int flags, mode_t mode);
int Close(int fd);
int Munmap(void *start, size_t length);
int Sem_init(sem_t *sem, int pshared, unsigned int value);
int P(sem_t *s);
int V(sem_t *s);

int Pthread_create(pthread_t *tid, pthread_attr_t *attr, func *f, void *arg);
/* 输出字符串*/
char *pInputNull;
#endif
