#include"DBFileManager.h"
#include"../utils/DBLog.h"
#include<cstring>
#include<cstdio>
#include<unistd.h>

DBFileManager::DBFileManager(const char* root):
    databaseName("test"), tableName(NULL), root(root), isOpened(false), recordInfo(NULL)
{
    databasePath = new char[256];
    strcpy(databasePath, "");
    strcat(databasePath, root);
    strcat(databasePath, "/");
    strcat(databasePath, databaseName);
    fileManager = new FileManager();
    bufPageManager = new BufPageManager(fileManager);
    log("File manager initialized.");
}

DBFileManager::~DBFileManager()
{
}

bool DBFileManager::opened()
{
    return isOpened;
}

void DBFileManager::useDatabase(const char* name)
{
    if(isOpened)
    {
        fileManager->closeFile(fileID);
    }
    log("switch to database " + string(name));
    databaseName = name;
    strcpy(databasePath, "");
    strcat(databasePath, root);
    strcat(databasePath, "/");
    strcat(databasePath, databaseName);
}

int DBFileManager::createTable(const char* name)
{
    char* fullname = new char[256];
    strcpy(fullname, "");
    strcat(fullname, root);
    if(access(fullname, F_OK) != 0)
    {
        mkdir(fullname, 0777);
    }
    strcat(fullname, "/");
    strcat(fullname, databaseName);
    if(access(fullname, F_OK) != 0)
    {
        mkdir(fullname, 0777);
        log("Created folder " + string(fullname));
    }
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
    tableName = name;
    strcpy(fullname, "");
    strcat(fullname, root);
    strcat(fullname, "/");
    strcat(fullname, databaseName);
    strcat(fullname, "/");
    strcat(fullname, tableName);
    if(access(fullname, F_OK) != 0)
    {
        printLine("File " + string(name) + " does not exist.");
        return OPEN_ERROR;
    }
    if(!fileManager->openFile(fullname, fileID))
    {
        return OPEN_ERROR;
    }
    log("Opened file " + string(fullname));
    isOpened = true;
    return SUCCEED;
}

void DBFileManager::setRecordInfo(std::vector<std::string> names, std::vector<int> types)
{
    if(recordInfo != NULL)
    {
        delete recordInfo;
    }
    recordInfo = new DBRecordInfo(names, types);
}

int DBFileManager::dropTable(const char* name)
{
    if(isOpened && string(name) == string(tableName))
    {
        closeTable();
    }
    char* fullname = new char[256];
    strcpy(fullname, "");
    strcat(fullname, root);
    strcat(fullname, "/");
    strcat(fullname, databaseName);
    strcat(fullname, "/");
    strcat(fullname, name);
    if(access(fullname, F_OK) != 0)
    {
        printLine("File " + string(name) + " does not exist.");
        return DROP_ERROR;
    }
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
    if(fileManager->closeFile(fileID) != 0)
    {
        return CLOSE_ERROR;
    }
    log("Closed file " + string(databasePath) + "/" + string(tableName));
    isOpened = false;
    return SUCCEED;
}

bool DBFileManager::createInfoPage(int _ID){
    char nameParameters[16];
    sprintf(nameParameters, "INFOPAGE%d", _ID);
    char* msg = new char[64];
    sprintf(msg, "File INFOPAGE%d created.", _ID);
    log(std::string(msg));
    return fileManager -> createFile(nameParameters);
}

bool DBFileManager::createDataPage(int _ID){
    char dataParameters[16];
    sprintf(dataParameters, "DATAPAGE%d", _ID);
    char* msg = new char[64];
    sprintf(msg, "FILE DATAPAGE%d created.", _ID);
    log(std::string(msg));
    return fileManager -> createFile(dataParameters);
}

bool DBFileManager::readInfoPage(int _ID, int &FileID){
    //TODO connect FileID to data stream
    char nameParameters[16];
    sprintf(nameParameters, "INFOPAGE%d", _ID);
    char* msg = new char[64];
    sprintf(msg, "File INFOPAGE%d opened.", _ID);
    log(std::string(msg));
    return fileManager -> openFile(nameParameters, FileID);
}

bool DBFileManager::readDataPage(int _ID, int & FileID){
    //TODO connect FileID to data stream
    char dataParameters[16];
    sprintf(dataParameters, "DATAPAGE%d", _ID);
    char* msg = new char[64];
    sprintf(msg, "FILE DATAPAGE%d opened.", _ID);
    log(std::string(msg));
    return fileManager -> openFile(dataParameters, FileID);
}

void DBFileManager::insertRecord(DBRecordInfo _infoTOInsert){

}

void DBFileManager::deleteRecord(DBRecordInfo _infoToDelete){

}

void DBFileManager::updateRecord(DBRecordInfo _infoToUpdate){

}

void DBFileManager::searchRecord(DBRecordInfo _infoTOSearch){

}