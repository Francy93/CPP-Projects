#include "musicShop.hpp"
#include "operations.cpp"


using namespace std;


int main(){
    bool exit = false;
    cout << "Welcome to the Music-Shop Software" << endl;

    while(!exit){
        exit = userFirst(exit);
        if(!exit){ exit = menuSecond(exit); }
    }
    // if there have been any changes which have not been saved yet, then ask for saving them
    if(!util.saved){
        cout << "WARNING! Before leaving. Do you wanna save unstored changes?\r\nEnter \"yes\" or \"no\"" << endl;
        while(true){
            string saving; cin >> saving;
            if (saving == "yes"){ util.updater();  break; }
            else if(saving == "no"){ break; }
            else { cout << "Choice not contemplated. Try again!" << endl; }
        }
    }
    cout << "\r\nSuccessfully exited!" << endl;
    return 0;
}