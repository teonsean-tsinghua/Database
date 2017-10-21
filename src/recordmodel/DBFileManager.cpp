#include"DBFileManager.h"

DBFileManager::DBFileManager(const char* root):
    root(root), isOpened(false), databaseName("test"), tableName(NULL)
{
    fileManager = new FileManager();
    bufPageManager = new BufPageManager(fileManager);
}

DBFileManager::~DBFileManager()
{
    delete bufPageManager;
    delete fileManager;
}

void DBFileManager::setDatabase(const char* name)
{
    if(isOpened)
    {
        fileManager.closeFile(fileID);
    }
    databaseName = name;
}
