#include <iostream>
#include <stdio.h> 
#include <unistd.h> 
#include "listener.h"
#include <thread>
#include <chrono>
#include <regex>
#include "json.hpp"
#include "global.h"
#include "menus.h"
using namespace std;
using json = nlohmann::json;

int main() 
{ 
	string buff;
	while(true){
		cout<<"ShellNo->";
		getline(cin,buff);
		// 49152-65535
		regex param("list -c ([1-5]?[0-9]?[0-9]?[0-9]?[0-9]|[6][0-5][0-5][0-3][0-5])");
		if(regex_match(buff,param)){
			smatch match;
			regex numb("[1-5]?[0-9]?[0-9]?[0-9]?[0-9]|[6][0-5][0-5][0-3][0-5]");
			regex_search(buff,match,numb);
			int port = stoi(match[0],nullptr,10);
			listManager->addListener(port);
			//t1.join();
		}
		if(regex_match(buff,regex("list -a"))){
			listManager->listListeners();
		}
		if(regex_match(buff,regex("list -l [1-9]?[1-9]"))){
			smatch match;
			regex numb("[1-9]?[1-9]");
			regex_search(buff,match,numb);
			int id = stoi(match[0],nullptr,10);
			postExploitMenu(listManager->getListener(id-1));
		}
		if (regex_match(buff, regex("exit"))) {
			break;
		}
		if(regex_match(buff, regex("help"))){
			printf("HELP MENU\n");
			printf("<========>\n");
			printf("list -c <port number>: listen on port\n");
			printf("list -a: list all listeners\n");
			printf("list -l <id>: connect to specific listener\n");
			printf("exit: exit ShellNo\n");
		}

	}

	return 0; 
} 
