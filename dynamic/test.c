#include <stdlib.h>
#include <stdio.h>

int get_param(char *argvs, char token, char *arg[])
{
	int i = 0;
	char *p = NULL;
	if(argvs == NULL)
	{
		return 0;
	}
	char *phead = argvs;
    while(phead != NULL)
    {
		if(*phead == '\0')
	        break;
	    i++;
	    p = strchr(phead,'&');
	    if(p == NULL)
	    {
	        arg[i-1] = phead; 
	        break;   	
		}
	    *p = '\0';
	    arg[i-1] = phead;
	    phead = p+1;
	}
	return i;
}

int main(int argc, char **argv, char **env)
{
    char *buf, *p = NULL;
    char *str = "100&100";
    char astr[100];
    memset(astr,0,sizeof(astr));
    memcpy(astr,str,strlen(str));
    char *arg[100];
    memset(arg,0,sizeof(arg));
    char **parg = NULL;
    buf = astr;
    printf("%s\n",buf);
    int ret = get_param(buf, '&', arg);
    printf("%d\n",ret);
    int i = 0;
    for(;i<ret;i++)
    {
        printf("%s\n",arg[i]);
    }
}


