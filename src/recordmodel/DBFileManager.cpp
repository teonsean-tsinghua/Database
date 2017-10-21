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

int DBFileManager::createTable(const char* name)
{
    char* fullName = new char[256];
    fullname = "";
    strcat(fullname, root);
    strcat(fullname, databaseName);
    strcat(fullname, tableName);
    if(!fileManager.createFile(fullName))
    {
        return CREATE_ERROR;
    }
    return SUCCEED;
}

int DBFileManager::openTable(const char* name)
{
    char* fullName = new char[256];
    fullname = "";
    strcat(fullname, root);
    strcat(fullname, databaseName);
    strcat(fullname, tableName);
    if(!fileManager.openFile(fullname, fileID))
    {
        return OPEN_ERROR;
    }
    isOpened = true;
    return SUCCEED;
}

int DBFileManager::closeTable()
{
    if(!isOpened)
    {
        return SUCCEED;
    }
    if(!fileManager.closeFile(fileID))
    {
        return CLOSE_ERROR;
    }
    isOpened = false;
    return SUCCEED;
}
