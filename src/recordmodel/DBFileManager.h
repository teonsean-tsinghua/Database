#ifndef FILEMANAGER_H_INCLUDED
#define FILEMANAGER_H_INCLUDED

#include"../bufmanager/BufPageManager.h"

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

    bool createFile(const char* name);

    ~DBFileManager();
};

#endif // FILEMANAGER_H_INCLUDED
