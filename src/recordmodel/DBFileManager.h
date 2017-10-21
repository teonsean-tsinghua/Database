#ifndef FILEMANAGER_H_INCLUDED
#define FILEMANAGER_H_INCLUDED

#include"../bufmanager/BufPageManager.h"

#include<cstring>

class DBFileManager
{
private:
    FileManager* fileManager;
    BufPageManager* bufPageManager;
    const char* databaseName;
    const char* tableName;
    const char* const root;
    int fileID;
    bool isOpened;  //Whether or not a file is opened

public:
    DBFileManager(const char* root);

    void setDatabase(const char* name);

    int createTable(const char* name);

    int openTable(const char* name);

    int closeTable();

    ~DBFileManager();

    const static int SUCCEED = 0;
    const static int CREATE_ERROR = SUCCEED + 1;
    const static int OPEN_ERROR = CREATE_ERROR + 1;
    const static int CLOSE_ERROR = OPEN_ERROR + 1;
};

#endif // FILEMANAGER_H_INCLUDED
