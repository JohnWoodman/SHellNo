#include <iostream>
#include <stdio.h> 
#include <unistd.h> 
#include "listener.h"
#include <thread>
#include <chrono>
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
		if(!buff.compare("create list")){
			listener* newL = new listener(3030);
			thread t1(&listener::test,newL);
			//t1.detach();
			t1.join();
		}
	}

	return 0; 
} 
