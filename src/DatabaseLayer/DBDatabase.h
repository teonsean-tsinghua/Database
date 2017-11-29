#ifndef DBDATABASE_H_INCLUDED
#define DBDATABASE_H_INCLUDED

#include"../DBInclude.h"
#include"../FileLayer/DBFileInclude.h"
#include"DBBase.h"
#include"y.tab.h"

class DBDataBase
{
private:
    static DBDataBase* instance;
    std::map<std::string, DBDataFile*> data;
    std::map<std::string, std::map<std::string, DBIndexFile*> > indexes;

    std::vector<std::string> pNames;
    std::vector<int> pTypes;
    std::vector<bool> pNullables;
    std::vector<int> pExtras;

    const std::string root;
    std::string name;

    DBDataBase(std::string root = "/home/teon/Documents/Database");

    void delete_path(const char* path);

    void _test();

public:
    static DBDataBase* getInstance();

    void createDatabase(std::string name);

    void dropDatabase(std::string name);

    void useDatabase(std::string name);

    void showDatabases();

    void createTable(std::string name);

    void dropTable(std::string name);

    void describeTable(std::string name);

    void showTables();

    void addPending(std::string name, int type, bool nullable, int extra);

    static void test();
};

#endif // DBDATABASE_H_INCLUDED


