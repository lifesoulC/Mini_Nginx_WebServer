#include "common.h"
// 
// name: 未知
// @param
// @return
//
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

int Get_param(char *argvs, char token, char *arg[])
{
    int ret = 0;
    if((ret = get_param(argvs, token, arg)) < 1)
    {
		printf("No Param!");
	}
}
