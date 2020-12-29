#include <iostream>
#include <stdio.h> 
#include <unistd.h> 
#include "listener.h"
#include <thread>
#include <chrono>
#include "json/single_include/nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;
#define MSGSIZE 1024
char msg1[] = "hello, world #1"; 

void hello(int in){
	int count=0;
	while(1){
		cout<<"hello"<<endl;
		this_thread::sleep_for(chrono::seconds(1));
	}
}

int main() 
{ 
	char buff[200];
	while(true){
		printf("ShellNo->");
		scanf("%s",buff);
		printf("%s\n",buff);
	}
	char inbuf[MSGSIZE]; 
	int p[2], pid, nbytes; 

	if (pipe(p) < 0) 
		exit(1); 

	/* continued */
	pid = fork();
	cout<<pid<<endl;
	if (pid!=0) { 
		while(true){
			scanf("%s",msg1);
			write(p[1], msg1, MSGSIZE); 
		}
		// Adding this line will 
		// not hang the program 
		close(p[1]); 
		wait(NULL); 
	} 

	else { 
		// Adding this line will 
		listener newL(3030);
		close(p[1]); 
		while ((nbytes = read(p[0], inbuf, MSGSIZE)) > 0){
			if(!strcmp(inbuf,"runs")){
				cout<<"You typed runs"<<endl;
			} else if (!strcmp(inbuf,"listen")) {
				thread t1(&listener::test,newL,4);
				t1.join();
				cout << "Thread finished" << endl;
			} else if (!strcmp(inbuf,"download")) {
				newL.downloadFile();
			}
		}
		if (nbytes != 0){
					exit(2); 
		}
		printf("Finished reading\n"); 
	} 
	return 0; 
} 
