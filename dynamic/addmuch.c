#include "../defs.h"
#include "../pub.h"
#include "common.h"
#define MAXCNT 100


int main(int argc, char **argv, char **env)
{
    char *buf = NULL;
    char *arg[MAXCNT];
    int cnt = 0;
    char content[MAXLINE];
    int result = 0;
    
    if((buf = getenv("QUERY_STR")) != NULL)
    {
		cnt = Get_param(buf, '&', arg);
	}
	int i = 0;
	while(i < cnt)
	{
		result += atoi(arg[i]);
		i++;
	}
	sprintf(content, "Welcome to addmuch.com:");
	sprintf(content, "%sThe Internet addition portal.\r\n<p>", content);
	sprintf(content, "%sThe answer is: ",content);
	sprintf(content, "%s %d ",content,atoi(arg[0]));
	i = 1;
	while(i < cnt)
	{
	    sprintf(content, "%s + %d",content, atoi(arg[i]));
	    i++;
    }
	sprintf(content, "%s = %d\r\n<p>",content,result);
	sprintf(content, "%sThanks for visiting!\r\n",content);
	
	printf("Content-length: %d\r\n", (int)strlen(content));
	printf("Content-type: text\html\r\n\r\n");
	printf("%s", content);
	fflush(stdout);
    exit(0);	
}
