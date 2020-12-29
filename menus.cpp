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

	cout << ">";
	while (cin >> cmd){
		if (cmd == "download") {
			printf("You Selected Download\n");
			newL->downloadFile();
		} else if (cmd == "upload") {
			printf("You selected Upload\n");
			newL->uploadFile("test3.txt");
		} else if (cmd == "exit") {
			printf("Exiting!\n");
			break;
		}
		cout << ">";
	}
}

