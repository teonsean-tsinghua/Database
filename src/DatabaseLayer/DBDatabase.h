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
    DBDataFile* data;
    std::map<std::string, DBIndexFile*> indexes;

    std::vector<std::string> pNames;
    std::vector<int> pTypes;
    std::vector<bool> pNullables;
    std::vector<int> pExtras;

    const char* const root;
    const char* name;

    DBDataBase(const char* root = "/home/teon/Documents/Database");

    void delete_path(const char* path);

    void _test();

public:
    static DBDataBase* getInstance();

    void createDatabase(const char* name);

    void dropDatabase(const char* name);

    void useDatabase(const char* name);

    void showDatabases();

    void showTables();

    void addPending(std::string name, int type, bool nullable, int extra);

    void dropTable(const char* name);

    static void test();
};

#endif // DBDATABASE_H_INCLUDED


