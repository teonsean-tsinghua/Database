#ifndef FILEMANAGER_H_INCLUDED
#define FILEMANAGER_H_INCLUDED

#include<../bufmanager/BufPageManager.h>
#include<string>

class DBFileManager
{
private:
    FileManager* fileManager;
    BufPageManager* bufPageManager;
    std::string databaseName;
    std::string tableName;
    std::string root;

public:
    DBFileManager(std::string root);

    ~DBFileManager();
};

#endif // FILEMANAGER_H_INCLUDED
