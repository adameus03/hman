#include <iostream>
#include "filenet.h"
#include <cstring>

//g++ main.cpp net.cpp -o hman -lws2_32

using namespace std;

int main(int argc, char** argv)
{
    cout << "Hello world!" << endl;
    if(argc==3){
        if(!strcmp(argv[1], "-r")){
            cout << "Receiver mode" << endl;
            receive_file(argv[2]);

        }
        else if(!strcmp(argv[1], "-t")){
            cout << "Transmiter mode" << endl;
            transmit_file(argv[2]);

        }
        else {
            cout << "Invalid mode" << endl;
        }
    }
    else {
        cout << "Wrong # of args" << endl;
    }
    return 0;
}
