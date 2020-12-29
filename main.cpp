#include <iostream>
#include <stdio.h> 
#include <unistd.h> 
#include "listener.h"
#include <thread>
#include <chrono>
#include <regex>
#include "json.hpp"
#include "global.h"
using namespace std;
using json = nlohmann::json;

//vector<listener*> listeners;

int main() 
{ 
	string buff;
	while(true){
		cout<<"ShellNo->";
		getline(cin,buff);
		// 49152-65535
		regex rgx("list -l ([1-5]?[0-9]?[0-9]?[0-9]?[0-9]|[6][0-5][0-5][0-3][0-5])");
		if(regex_match(buff,r)){
			smatch match;
			regex_search(buff.begin(),buff.end(),match,r);
			cout<<"matched"<<endl;
		}
		if(!buff.compare("list -")){
			listener* newL = new listener(3030);
			listeners->push_back(newL);
			thread t1(&listener::test,newL);
			t1.detach();
		}
	}

	return 0; 
} 
