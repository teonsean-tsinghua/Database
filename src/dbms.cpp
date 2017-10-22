#include<iostream>
#include<string>
#include<cstdlib>
#include"recordmodel/DBFileManager.h"
#include"utils/DBLog.h"
using namespace std;

int main()
{
    DBFileManager fm("/home/teon/Documents/Database");
    while(true)
    {
        string command;
        print(">>");
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
            print("Please enter name of the file: ");
            string filename;
            getline(cin, filename);
            fm.createTable(filename.c_str());
            int count = 0;
            printLine("Please define the columns of records you want to store in this file.");
            vector<string> names;
            vector<int> types;
            while(count < 31)
            {
                string name;
                print("Please enter name of the column, or \"DBSTOP\" to finish: ");
                getline(cin, name);
                if(name.compare("DBSTOP") == 0)
                {
                    break;
                }
                names.push_back(name);
                string type;
                print("Please select type of the data \n(1 for int, 2 for long, 3 for boolean, 4 for char, 5 for float, 6 for double, 7 for string, 8 for long string): ");
                getline(cin, type);
                types.push_back(atoi(type.c_str()));
            }
        }
        else if(command.compare("deleteFile") == 0)
        {
            print("Please enter name of the file: ");
            string filename;
            getline(cin, filename);
            fm.dropTable(filename.c_str());
        }
        else if(command.compare("closeFile") == 0)
        {
            if(!fm.opened())
            {
                printLine("You have not opened any file yet.");
                continue;
            }
            fm.closeTable();
        }
        else if(command.compare("openFile") == 0)
        {
            if(fm.opened())
            {
                printLine("Please close the currently opened file first.");
                continue;
            }
            print("Please enter name of the file: ");
            string filename;
            getline(cin, filename);
            fm.openTable(filename.c_str());
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
