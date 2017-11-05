#include "DBFileManager.h"
#include "../utils/DBLog.h"
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <dirent.h>

DBFileManager::DBFileManager(const char* root):
    databaseName(""), tableName(""), root(root), isOpened(false)
{
    databasePath = new char[512];
    strcpy(databasePath, "");
    strcat(databasePath, root);
    strcat(databasePath, "/");
    strcat(databasePath, "");
    fileManager = new FileManager();
    bufPageManager = new BufPageManager(fileManager);
    recordInfoParser = new DBRecordInfo();
    recordUsageParser = new DBRecordUsage();
    recordParser = new DBRecord();
    log("File manager initialized.");
}

DBFileManager::~DBFileManager()
{
}

bool DBFileManager::opened()
{
    return isOpened;
}

int DBFileManager::createDatabase(const char* name)
{
    if (isOpened){
        fileManager -> closeFile(fileID);
    }
    if (strcmp(databaseName, "") != 0){
        return CREATE_ERROR;
    }
    char* dbname = new char[512];
    if (strlen(name) > 250){
        return CREATE_ERROR;
    }
    strcpy(dbname, "");
    strcat(dbname, root);
    if (access(dbname, F_OK) != 0){
        cout << "error1" << endl;
        return CREATE_ERROR;
    }
    strcat(dbname, "/");
    strcat(dbname, name);
    if (strlen(dbname) > 250){
        return CREATE_ERROR;
    }
    if (access(dbname, F_OK) != 0){
        mkdir(dbname, 0777);
        log("Created database" + string(name));
        return SUCCEED;
    }
    return CREATE_ERROR;
}

int DBFileManager::openDatabase(const char* name)
{
    if(isOpened)
    {
        fileManager->closeFile(fileID);
    }
    if(strlen(databaseName) > 0){
        return OPEN_ERROR;
    }
    log("switch to database " + string(name));
    databaseName = name;
    strcpy(databasePath, "");
    strcat(databasePath, root);
    strcat(databasePath, "/");
    strcat(databasePath, name);
    return SUCCEED;
}

int DBFileManager::createTable(const char* name)
{
    // Note: the table here created will be empty, needs furthur implementation to initialize its contents.
    char* fullname = new char[512];
    strcpy(fullname, "");
    strcat(fullname, root);
    if(access(fullname, F_OK) != 0)
    {
        return CREATE_ERROR;
    }
    strcat(fullname, "/");
    strcat(fullname, databaseName);
    if(access(fullname, F_OK) != 0)
    {
        return CREATE_ERROR;
    }
    strcat(fullname, "/");
    strcat(fullname, name);
    if (strlen(fullname) > 250){
        return CREATE_ERROR;
    }
    if(access(fullname, F_OK) != 0)
    {
        mkdir(fullname, 0777);
        log("Created table " + string(name));
        char* dataFileName = new char[512];
        strcpy(dataFileName, fullname);
        strcat(dataFileName, "/data");
        fileManager -> createFile(dataFileName);
        char* usageFileName = new char[512];
        strcpy(usageFileName, fullname);
        strcat(usageFileName, "/usage");
        fileManager -> createFile(usageFileName);
        char* descriptionFileName = new char[512];
        strcpy(descriptionFileName, fullname);
        strcat(descriptionFileName, "/description");
        fileManager -> createFile(descriptionFileName);
        tableName = name;
        initializeUsageFile();
        unsigned char* emptydescription = new unsigned char[PAGE_SIZE];
        writeToDescriptionPage(emptydescription);
        tableName = "";
        return SUCCEED;
    }
    return CREATE_ERROR;
}

int DBFileManager::openTable(const char* name)
{
    if(strcmp(databaseName, "") == 0){
        return OPEN_ERROR;
    }
    int fileID;
    char* fullname = new char[512];
    tableName = name;
    strcpy(fullname, "");
    strcat(fullname, root);
    strcat(fullname, "/");
    strcat(fullname, databaseName);
    strcat(fullname, "/");
    strcat(fullname, tableName);
    if(access(fullname, F_OK) != 0)
    {
        printLine("Folder " + string(name) + " does not exist.");
        return OPEN_ERROR;
    }
    if(!fileManager->openFile(fullname, fileID))
    {
        return OPEN_ERROR;
    }
    log("Opened folder " + string(fullname));
    recordInfoParser -> fromBinary(readDescriptionPage());
    recordUsageParser -> setData(readUsagePage());
    return SUCCEED;
}

int DBFileManager::dropTable(const char* name)
{
    if (strcmp(databaseName, "") == 0){
        return DROP_ERROR;
    }
    if(isOpened && string(name) == string(tableName))
    {
        closeTable();
    }
    char* fullname = new char[512];
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
    log("Deleted table " + string(fullname));
    return SUCCEED;
}

int DBFileManager::dropDatabase(const char* name){
    if (isOpened && string(name) == string(databaseName)){
        closeDatabase();
    }
    char* fullname = new char[512];
    strcpy(fullname, "");
    strcpy(fullname, root);
    strcpy(fullname, "/");
    strcpy(fullname, databaseName);
    if(access(fullname, F_OK) != 0){
        printLine("Database " + string(name) + " does not exist.");
        return DROP_ERROR;
    }
    if (remove(name) != 0){
        printLine("Error when closing " + string(name));
        return DROP_ERROR;
    }
    log("Deleted Database" + string(databaseName));
    return SUCCEED;
}

int DBFileManager::closeDatabase(){
    if (!isOpened){
        return SUCCEED;
    }
    if (fileManager -> closeFile(fileID) != 0){
        return CLOSE_ERROR;
    }
    log("Close database " + string(databaseName));
    databaseName = "";
    isOpened = false;
    return SUCCEED;
}

int DBFileManager::closeTable()
{
    if(strcmp(databaseName, "") == 0){
        return CLOSE_ERROR;
    }
    if(!isOpened)
    {
        return SUCCEED;
    }
    if(fileManager->closeFile(fileID) != 0)
    {
        return CLOSE_ERROR;
    }
    log("Closed file " + string(databasePath) + "/" + string(tableName));
    tableName = "";
    isOpened = false;
    return SUCCEED;
}

int DBFileManager::listDatabase(){
    char* rootname = new char[512];
    strcpy(rootname, "");
    strcat(rootname, root);
    strcat(rootname, "/");
    struct dirent *ent = NULL;
    DIR *dir = opendir(rootname);
    if(dir == NULL){
        return LIST_ERROR;
    }
    while((ent = readdir(dir)) != NULL){
        if (ent -> d_name[0] != '.'){
            printf("%s\n", ent-> d_name);
        }
    }
    closedir(dir);
    return SUCCEED;
}

int DBFileManager::listTable(){
    if(strcmp(databaseName, "") == 0){
        return LIST_ERROR;
    }
    char* dbname = new char[512];
    strcpy(dbname, "");
    strcat(dbname, root);
    strcat(dbname, "/");
    strcat(dbname, databaseName);
    strcat(dbname, "/");
    struct dirent *ent = NULL;
    DIR *dir = opendir(dbname);
    if(dir == NULL){
        return LIST_ERROR;
    }
    while((ent = readdir(dir)) != NULL){
        if (ent -> d_name[0] != '.'){
            printf("%s\n", ent-> d_name);
        }
    }
    closedir(dir);
    return SUCCEED;
}

unsigned char* DBFileManager::readDataPage(int pageID){
    if(strcmp(tableName, "") == 0){
        return NULL;
    }
    unsigned char* ret = new unsigned char[PAGE_SIZE];
    char* dataPageName = new char[512];
    strcpy(dataPageName, "");
    strcat(dataPageName, root);
    strcat(dataPageName, "/");
    strcat(dataPageName, databaseName);
    strcat(dataPageName, "/");
    strcat(dataPageName, tableName);
    strcat(dataPageName, "/data");
    int fileID;
    fileManager -> openFile(dataPageName, fileID);
    if (fileManager -> readPage(fileID, pageID, (unsigned int*)ret, 0) != 0){
        return NULL;
    }
    fileManager -> closeFile(fileID);
    return ret;
}

unsigned char* DBFileManager::readDescriptionPage(){
    if(strcmp(tableName, "") == 0){
        return NULL;
    }
    unsigned char* ret = new unsigned char[PAGE_SIZE];
    char* descriptionPageName = new char[512];
    strcpy(descriptionPageName, "");
    strcat(descriptionPageName, root);
    strcat(descriptionPageName, "/");
    strcat(descriptionPageName, databaseName);
    strcat(descriptionPageName, "/");
    strcat(descriptionPageName, tableName);
    strcat(descriptionPageName, "/description");
    int fileID, pageID;
    pageID = 0;
    fileManager -> openFile(descriptionPageName, fileID);
    if (fileManager -> readPage(fileID, pageID, (unsigned int*)ret, 0) != 0){
        return NULL;
    }
    fileManager -> closeFile(fileID);
    return ret;
}

unsigned char* DBFileManager::readUsagePage(){
    if(strcmp(tableName, "") == 0){
        return NULL;
    }
    unsigned char* ret = new unsigned char[PAGE_SIZE];
    char* usagePageName = new char[512];
    strcpy(usagePageName, "");
    strcat(usagePageName, root);
    strcat(usagePageName, "/");
    strcat(usagePageName, databaseName);
    strcat(usagePageName, "/");
    strcat(usagePageName, tableName);
    strcat(usagePageName, "/usage");
    int fileID, pageID;
    pageID = 0;
    fileManager -> openFile(usagePageName, fileID);
    if (fileManager -> readPage(fileID, pageID, (unsigned int*)ret, 0) != 0){
        return NULL;
    }
    fileManager -> closeFile(fileID);
    return ret;
}

unsigned char* DBFileManager::readIndexPage(const char* indexName, int pageId){}

int DBFileManager::writeToIndexPage(const char* indexName, int pageID){
    return SUCCEED;
};

int DBFileManager::writeToDataPage(const unsigned char* data, int pageID){
    if(strcmp(tableName, "") == 0){
        return WRITE_ERROR;
    }
    char* dataFileName = new char[512];
    strcpy(dataFileName, "");
    strcat(dataFileName, root);
    strcat(dataFileName, "/");
    strcat(dataFileName, databaseName);
    strcat(dataFileName, "/");
    strcat(dataFileName, tableName);
    strcat(dataFileName, "/data");
    int fileID;
    if (fileManager -> openFile(dataFileName, fileID) == 0){
        return WRITE_ERROR;
    }
    if (fileManager -> writePage(fileID, pageID, (unsigned int*)data, 0) != 0){
        return WRITE_ERROR;
    }
    fileManager -> closeFile(fileID);
    return SUCCEED;
}

int DBFileManager::writeToDescriptionPage(const unsigned char* data){
    if(strcmp(tableName, "") == 0){
        return WRITE_ERROR;
    }
    char* descriptionFileName = new char[512];
    strcpy(descriptionFileName, "");
    strcat(descriptionFileName, root);
    strcat(descriptionFileName, "/");
    strcat(descriptionFileName, databaseName);
    strcat(descriptionFileName, "/");
    strcat(descriptionFileName, tableName);
    strcat(descriptionFileName, "/description");
    int fileID;
    if (fileManager -> openFile(descriptionFileName, fileID) == 0){
        return WRITE_ERROR;
    }
    if (fileManager -> writePage(fileID, 0, (unsigned int*)data, 0) != 0){
        return WRITE_ERROR;
    }
    fileManager -> closeFile(fileID);
    return SUCCEED;
}

int DBFileManager::writeToUsagePage(const unsigned char* data){
    if(strcmp(tableName, "") == 0){
        return WRITE_ERROR;
    }
    char* usageFileName = new char[512];
    strcpy(usageFileName, "");
    strcat(usageFileName, root);
    strcat(usageFileName, "/");
    strcat(usageFileName, databaseName);
    strcat(usageFileName, "/");
    strcat(usageFileName, tableName);
    strcat(usageFileName, "/usage");
    int fileID;
    if (fileManager -> openFile(usageFileName, fileID) == 0){
        return WRITE_ERROR;
    }
    if (fileManager -> writePage(fileID, 0, (unsigned int*)data, 0) != 0){
        return WRITE_ERROR;
    }
    fileManager -> closeFile(fileID);
    return SUCCEED;
}

void DBFileManager::initializeUsageFile(){
    unsigned char* usage = new unsigned char[PAGE_SIZE];
    for(int i = 0; i < (PAGE_SIZE >> 1); i++){
        usage[i * 2] = 32;
        usage[i * 2 + 1] = 0;
    }
    writeToUsagePage(usage);
}

int DBFileManager::insertData(std::vector<std::string> &data, std::vector<int> &len){
    unsigned char* binData = recordInfoParser -> parseData(data, len);
    int pageIdx = recordUsageParser -> allocateNewRecord(recordInfoParser -> getRecordLen());
    recordParser -> setData(readDataPage(pageIdx), recordInfoParser -> getRecordLen(), 
        PAGE_SIZE - (recordUsageParser -> getUsageByPage(pageIdx)));
    recordParser -> insertData(binData);
    writeToDataPage(recordParser -> getData(), pageIdx);
    writeToUsagePage(recordUsageParser -> getData());
    return SUCCEED;
}

int DBFileManager::deleteData(std::vector<unsigned long long>* RID_ARRAY){
    for(int i = 0; i < (PAGE_SIZE / 2); i++){
        if ((PAGE_SIZE - recordUsageParser -> getUsageByPage(i)) > 0){
            recordParser -> setData(readDataPage(i), recordInfoParser -> getRecordLen(),
                PAGE_SIZE - (recordUsageParser -> getUsageByPage(i)));
            for(int j = 0; j < RID_ARRAY -> size(); j++)
                if (recordParser -> clearDataByRid((*RID_ARRAY)[j]) == true){
                    recordUsageParser -> releaseSize(i, recordInfoParser -> getRecordLen());
                    writeToDataPage(recordParser -> getData(), i);
                }
        }
    }
    writeToUsagePage(recordUsageParser -> getData());
    return SUCCEED;
}

std::vector<unsigned long long>* DBFileManager::getDataByKey(const unsigned char* keyName, const unsigned char* value, int valuelen){
    std::vector<unsigned long long>* ret = new std::vector<unsigned long long>();
    int offset = recordInfoParser -> getOffsetByName((char*)keyName);
    for(int i = 0; i < (PAGE_SIZE / 2); i++){
        if ((PAGE_SIZE - recordUsageParser -> getUsageByPage(i)) > 0){
            recordParser -> setData(readDataPage(i), recordInfoParser -> getRecordLen(),
                PAGE_SIZE - (recordUsageParser -> getUsageByPage(i)));
            vector<unsigned long long>* retthispage = recordParser -> searchData(value, offset, valuelen);
            ret -> insert(ret -> end(), retthispage -> begin(), retthispage -> end());
        }
    }
    return ret;
}