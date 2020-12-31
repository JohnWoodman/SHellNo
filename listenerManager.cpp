#include "listenerManager.h"

listenerManager::listenerManager(){
    
}

int
listenerManager::addListener(int port){
    listener* newListener = new listener(port);
    listeners.push_back(newListener);
    thread t1(&listener::test,newListener);
    t1.detach();
    return 1;
}

void 
listenerManager::listListeners(){
    for(int i=0;i<listeners.size();i++){
        cout<<listeners[i]->print()<<endl;
    }
}

listener* 
listenerManager::getListener(int id){
    return listeners[id];
}