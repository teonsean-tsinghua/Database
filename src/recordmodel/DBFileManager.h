#ifndef DBFILEMANAGER_H_INCLUDED
#define DBFILEMANAGER_H_INCLUDED

#include"../bufmanager/BufPageManager.h"
#include"DBFileInfo.h"
extern int tt;
extern unsigned char h[61];

//TODO: Make it singleton.

class DBFileManager
{
private:
    FileManager* fileManager;
    BufPageManager* bufPageManager;
    const char* databaseName;
    const char* tableName;
    const char* const root;
    char* databasePath;
    int fileID;
    bool isOpened;  //Whether or not a file is opened

public:
    DBFileManager(const char* root);

    void useDatabase(const char* name);

    int createTable(const char* name);

    int openTable(const char* name);

    int dropTable(const char* name);

    int closeTable();

    bool opened();

    ~DBFileManager();

    const static int SUCCEED = 0;
    const static int CREATE_ERROR = SUCCEED + 1;
    const static int OPEN_ERROR = CREATE_ERROR + 1;
    const static int CLOSE_ERROR = OPEN_ERROR + 1;
    const static int DROP_ERROR = CLOSE_ERROR + 1;
};

#endif // DBFILEMANAGER_H_INCLUDED
