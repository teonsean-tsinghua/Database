#ifndef DBDATABASE_H_INCLUDED
#define DBDATABASE_H_INCLUDED

#include"../DBInclude.h"
#include"../FileLayer/DBFileInclude.h"
#include"DBBase.h"
#include"y.tab.h"

class DBDatabase
{
private:
    static DBDatabase* instance;
    std::map<std::string, DBDataFile*> data;
    std::map<std::string, std::map<std::string, DBIndexFile*> > indexes;

    std::vector<std::string> pNames;
    std::vector<int> pTypes;
    std::vector<bool> pNullables;
    std::vector<int> pExtras;
    std::vector<Value> pValues;
    std::vector<std::vector<Value> > pValueLists;

    const std::string root;
    std::string name;

    DBDatabase(std::string root = "/home/teon/Documents/Database");

    void delete_path(const char* path);

    void _test();

    bool databaseAvailable();

    DBDataFile* getDataFile(std::string name);

public:
    static DBDatabase* getInstance();

    void createDatabase(std::string name);

    void dropDatabase(std::string name);

    void useDatabase(std::string name);

    void showDatabases();

    void createTable(std::string name);

    void dropTable(std::string name);

    void describeTable(std::string name);

    void showTables();

    void addPendingField(std::string& name, int type, bool nullable, int extra);

    void addPendingValue(Value& value);

    void addPendingValueList();

    void clearPending();

    void insert(std::string name);

    static void test();
};

#endif // DBDATABASE_H_INCLUDED


