#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc, char* argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char message[]="hello world";
	char server_reply[2000];
	if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){
		printf("failed");
		exit(1);
	}
	if((s=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET){
		printf("couldnt create socket");
		exit(1);
	}
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(6969);
	if(connect(s,(struct sockaddr*)&server,sizeof(server))<0){
		printf("fialed to connect");
		exit(1);
	}
	if(send(s,message,strlen(message),0)<0){
		printf("failed to send");
		exit(1);
	}
	int recv_size;
	while(1){
		if((recv_size = recv(s,server_reply,2000,0))!=SOCKET_ERROR){
			server_reply[recv_size] = '\0';
			printf("%s\n",server_reply);
		}
		printf("asd");
		sleep(1);
	}
	return 0;
}