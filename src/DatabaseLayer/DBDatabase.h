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
    std::vector<std::string> pTables;
    std::vector<std::string> pColumns;
    std::vector<Col> pCols;
    std::vector<Where> pWheres;

    const std::string root;
    std::string name;

    DBDatabase(std::string root = "/home/teon/Documents/Database");

    void delete_path(const char* path);

    void _test();

    bool databaseAvailable();

    DBDataFile* getDataFile(std::string name);

    void selectOneTable(bool all);

    void selectMultiTable(bool all);

    void processWheresWithOneTable(SearchInfo& si, DBRecordInfo* ri);

    void printOneTableSelectResult(SelectResult& sr, std::vector<bool>& selected, DBRecordInfo* ri);

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

    void addPendingTable(std::string& name);

    void addPendingColumn(std::string& name);

    void addPendingCol(Col& col);

    void addPendingValue(Value& value);

    void addPendingValueList();

    void addPendingWhere(Where& where);

    void insert(std::string name);

    void select(bool all);

    static void test();
};

#endif // DBDATABASE_H_INCLUDED


