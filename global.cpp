#include "global.h"

vector<listener*>* listeners = new std::vector<listener*>();
const int MAX_LISTENERS = 32;
const int MSGSIZE  = 1024;
