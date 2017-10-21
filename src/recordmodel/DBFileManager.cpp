#include"DBFileManager.h"
#include"../utils/DBLog.h"
#include<cstring>
#include<cstdio>

DBFileManager::DBFileManager(const char* root):
    databaseName("test"), tableName(NULL), root(root), isOpened(false)
{
    databasePath = new char[256];
    databasePath = "";
    strcat(databasePath, root);
    strcat(databasePath, "/");
    strcat(databasePath, databaseName);
    fileManager = new FileManager();
    bufPageManager = new BufPageManager(fileManager);
    log("File manager initialized.");
}

DBFileManager::~DBFileManager()
{
    delete bufPageManager;
    delete fileManager;
}

void DBFileManager::useDatabase(const char* name)
{
    if(isOpened)
    {
        fileManager->closeFile(fileID);
    }
    log("switch to database " + string(name));
    databaseName = name;
}

int DBFileManager::createTable(const char* name)
{
    char* fullname = new char[256];
    fullname = "";
    strcat(fullname, root);
    strcat(fullname, "/");
    strcat(fullname, databaseName);
    strcat(fullname, "/");
    strcat(fullname, name);
    if(!fileManager->createFile(fullname))
    {
        return CREATE_ERROR;
    }
    log("Created file " + string(fullname));
    return SUCCEED;
}

int DBFileManager::openTable(const char* name)
{
    char* fullname = new char[256];
    fullname = "";
    tableName = name;
    strcat(fullname, root);
    strcat(fullname, "/");
    strcat(fullname, databaseName);
    strcat(fullname, "/");
    strcat(fullname, tableName);
    if(!fileManager->openFile(fullname, fileID))
    {
        return OPEN_ERROR;
    }
    log("Opened file " + string(fullname));
    isOpened = true;
    return SUCCEED;
}

int DBFileManager::dropTable(const char* name)
{
    if(string(name) == string(tableName))
    {
        closeTable();
    }
    char* fullname = new char[256];
    fullname = "";
    strcat(fullname, root);
    strcat(fullname, "/");
    strcat(fullname, databaseName);
    strcat(fullname, "/");
    strcat(fullname, name);
    if(remove(fullname) != 0)
    {
        return DROP_ERROR;
    }
    log("Deleted file " + string(fullname));
    return SUCCEED;
}

int DBFileManager::closeTable()
{
    if(!isOpened)
    {
        return SUCCEED;
    }
    if(!fileManager->closeFile(fileID))
    {
        return CLOSE_ERROR;
    }
    log("Closed file " + string(fullname));
    isOpened = false;
    return SUCCEED;
}
