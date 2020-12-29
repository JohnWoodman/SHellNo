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

//vector<listener*> listeners;

int main() 
{ 
	string buff;
	while(true){
		cout<<"ShellNo->";
		getline(cin,buff);
		if(!buff.compare("create list")){
			listener* newL = new listener(3030);
			newL->test();
		}
	}

	return 0; 
} 
