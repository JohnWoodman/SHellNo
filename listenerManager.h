#include <vector>
#include "listener.h"
using namespace std;

class listenerManager{
    public:
    listenerManager();
    int addListener(int port);
    void listListeners();
    listener* getListener(int id);
    private:
    vector<listener*> listeners;
};
