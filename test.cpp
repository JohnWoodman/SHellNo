#include <iostream>
#include <string>
#include <regex>
using namespace std;

int main()
{
    string a = "ricardo";
    while(1){
        cin>>a;
        regex r("[A-Z]:/([0-9a-zA-Z]+/)*([0-9a-zA-Z]+[.][0-9a-zA-Z]+)");
        if(regex_match(a,r)){
            cout<<"matched"<<endl;
        }
    }
    return 0;
}

