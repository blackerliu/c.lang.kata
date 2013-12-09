/*  
* 获取某个域名的ip地址,以列表形式打印。  
*  
* 作者: nswish  
* 2012-01-30  
*  
* 在如下环境编译通过：  
* KUbuntu 11.1  
* gcc version 4.6.1  
*/  
#include <stdio.h>   
#include <netdb.h>   
#include <stdlib.h>   
  
main(int argc, char *argv[]){   
    struct addrinfo * res, *pt;   
    struct sockaddr_in *sinp;   
    const char *addr;   
    char abuf[INET_ADDRSTRLEN];   
    int succ=0,i=0;   
  
    if(argc == 1)   
    {   
        printf("USAGE: getip <server name>\nFor example:\n    getip www.google.com\n");   
        exit(1);   
    }   
  
    succ = getaddrinfo(argv[1], NULL, NULL, &res);   
    if(succ != 0)   
    {   
        printf("Can't get address info! error code = %d", succ);   
        exit(succ);   
    }   
  
    for(pt=res, i=0; pt != NULL; pt=pt->ai_next, i++){   
        sinp = (struct sockaddr_in *)pt->ai_addr;   
        addr = (const char *)inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);   
        printf("%2d. IP=%s\n", i, addr);   
    }   
}  
