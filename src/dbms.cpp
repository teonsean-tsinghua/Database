#include<iostream>
#include<string>
#include"recordmodel/DBFileManager.h"
using namespace std;

int main()
{

    while(true)
    {
        //char* command = new char[256];
        string command;
        cout << "\n>>";
        cin >> command;
        cout << command << endl;
        if(command.compare("quit") == 0)
        {
            break;
        }
        if(command.compare("createFile") == 0)
        {
            cout << "\nPlease enter name of the file: ";
            string filename;
            cin >> filename;
        }
    }
    return 0;
}
