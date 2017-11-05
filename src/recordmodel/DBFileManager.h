#ifndef DBFILEMANAGER_H_INCLUDED
#define DBFILEMANAGER_H_INCLUDED

#include"../bufmanager/BufPageManager.h"
#include "DBRecordInfo.h"
#include "DBRecordUsage.h"
#include "DBRecord.h"
#include<vector>
#include<string>
extern int tt;
extern unsigned char h[61];

//TODO: Make it singleton.

class DBFileManager
{
private:
    FileManager* fileManager;
    BufPageManager* bufPageManager;
    DBRecordInfo* recordInfoParser;
    DBRecordUsage* recordUsageParser;
    DBRecord* recordParser;
    const char* databaseName;
    const char* tableName;
    const char* const root;
    char* databasePath;
    int fileID;
    bool isOpened;  //Whether or not a file is opened
    void initializeUsageFile();

public:
    DBFileManager(const char* root);

    int openDatabase(const char* name);
    int createDatabase(const char* name);
    int createTable(const char* name);

    int openTable(const char* name);
    int dropTable(const char* name);
    int dropDatabase(const char* name);

    int closeDatabase();
    int closeTable();

    int listDatabase();
    int listTable();

    unsigned char* readDataPage(int pageID);
    unsigned char* readDescriptionPage();
    unsigned char* readUsagePage();
    unsigned char* readIndexPage(const char* indexName, int pageID, int offset, int length);//TODO

    int writeToDataPage(const unsigned char* data, int pageID);
    int writeToDescriptionPage(const unsigned char* data);
    int writeToUsagePage(const unsigned char* data);
    int writeToIndexPage(const unsigned char* data, int length);//TODO

    int insertData(std::vector<std::string> &data, std::vector<int> &len);
    int deleteData(std::vector<unsigned long long> *RID_ARRAY);
    std::vector<unsigned long long>* getDataByKey(const unsigned char* keyName, const unsigned char* value, int valuelen);

    bool opened();

    ~DBFileManager();

    const static int SUCCEED = 0;
    const static int CREATE_ERROR = SUCCEED + 1;
    const static int OPEN_ERROR = CREATE_ERROR + 1;
    const static int CLOSE_ERROR = OPEN_ERROR + 1;
    const static int DROP_ERROR = CLOSE_ERROR + 1;
    const static int LIST_ERROR = DROP_ERROR + 1;
    const static int WRITE_ERROR = LIST_ERROR + 1;
    const static int READ_ERROR = WRITE_ERROR + 1;
    const static int INSERT_FAIL = READ_ERROR + 1;
    const static int DELETE_FAIL = INSERT_FAIL + 1;
    const static int SEARCH_FAIL = DELETE_FAIL + 1;
};

#endif // DBFILEMANAGER_H_INCLUDED
