#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

const int DATASIZE = 256;
int main(int argc, char* argv[])
{
    char PORT[] = "6969";
    char buf[DATASIZE];
    int sfd;
    struct addrinfo hints, *servinfo, *p;
    if(argc !=2 ){
        fprintf(stderr,"usage: client hostname/ipaddr\n");
        exit(1);
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int res;
    res = getaddrinfo(argv[1],PORT, &hints, &servinfo);
    if(res != 0){
        fprintf(stderr, "error getting address");
        exit(1);
    }
    for(p = servinfo; p!=NULL; p=p->ai_next){
        sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(sfd == -1) continue;
        if(connect(sfd,p->ai_addr, p->ai_addrlen) !=-1) break;
        close(sfd);
    }
    if(p==NULL){
        fprintf(stderr,"couldnt connect");
        exit(1);
    }
    freeaddrinfo(servinfo);
    int numbytes = recv(sfd,buf,DATASIZE-1,0);
    if(numbytes == -1){
        fprintf(stderr, "recv error");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("the response was %s", buf);
    close(sfd);
    return 0;
}