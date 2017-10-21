#include<iostream>
#include<string>
#include"recordmodel/DBFileManager.h"
using namespace std;

int main()
{
    DBFileManager fm("/home/teon/Document/Database");
    while(true)
    {
        string command;
        cout << "\n>>";
        getline(cin, command);
        if(command.compare("") == 1)
        {
            continue;
        }
        else if(command.compare("quit") == 0)
        {
            break;
        }
        else if(command.compare("createFile") == 0)
        {
            cout << "\nPlease enter name of the file: ";
            string filename;
            getline(cin, filename);
        }
        else if(command.compare("deleteFile") == 0)
        {
            cout << "\nPlease enter name of the file: ";
            string filename;
            getline(cin, filename);
        }
        else if(command.compare("closeFile") == 0)
        {
            cout << "\nPlease enter name of the file: ";
            string filename;
            getline(cin, filename);
        }
        else if(command.compare("updateFile") == 0)
        {
            cout << "\nPlease enter name of the file: ";
            string filename;
            getline(cin, filename);
        }
        else if(command.compare("insertRecord") == 0)
        {
        }
        else if(command.compare("deleteRecord") == 0)
        {
        }
        else if(command.compare("updateRecord") == 0)
        {
        }
        else if(command.compare("findRecord") == 0)
        {
        }
    }
    return 0;
}
