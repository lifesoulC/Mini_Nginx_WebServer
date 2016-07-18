#include "common.h"

char *pInputNull = "Input Param is NULL!";


void Error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);	
}
pid_t Fork(void)
{
    pid_t pid;
    
    if((pid = fork()) < 0)
    {
	    Error("fork error");
	}
	return pid;	
}

int Dup2(int oldfd, int newfd)
{
	int ret = 0;
	if((ret = dup2(oldfd, newfd)) < 0)
	    Error("dup2 error");
	return ret;
}

int Execve(const char *filename, const char *argv[], const char *envp[])
{
	int ret = 0;
	if((ret = execve(filename, argv, envp)) < 0)
	    Error("execve error");
	return ret;
}

int Setenv(const char *name, const char *newvalue, int overwrite)
{
    int ret = 0;
    if((ret = setenv(name, newvalue, overwrite)) < 0)
        Error("setenv error");
    return ret;	
}

int Open(const char *filename, int flags, mode_t mode)
{
	int ret = 0;
	if((ret = open(filename, flags, mode)) == -1)
	    Error("open error");
	return ret;
}

int Close(int fd)
{
	int ret = 0;
	if((ret = close(fd)) == -1)
	    Error("close error");
	return ret;
}
#if 0
void *mmap(void *start, size_t length, int port, int flags, int fd, off_t offset)
{
	return mmap();
}
#endif

int Munmap(void *start, size_t length)
{
	int ret = 0;
	if((ret = munmap(start, length)) == -1)
	    Error("unmap error");
	return ret;
}

int Sem_init(sem_t *sem, int pshared, unsigned int value)
{
    int ret = 0;
    if((ret = sem_init(sem, 0, value)) == -1)
        Error("sem init error");
    return ret;	
}

int P(sem_t *s)
{
	int ret = 0;
	if((ret = sem_wait(s)) == -1)
	    Error("sem wait error");
	return ret;
}

int V(sem_t *s)
{
	int ret = 0;
	if((ret = sem_post(s)) == -1)
	    Error("sem post error");
	return ret;
}

int Pthread_create(pthread_t *tid, pthread_attr_t *attr, func *f, void *arg)
{
	int ret = 0;
	ret = pthread_create(tid, attr, f, arg);
	if(ret != 0)
		Error("thread create error");
	printf("create thread, it's id is %lld\n",*tid);
    return ret;
}
