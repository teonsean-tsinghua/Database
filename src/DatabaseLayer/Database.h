#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include"Include.h"
#include"Base.h"
#include"y.tab.h"

class Database
{
private:
    static Database* instance;
    std::map<std::string, DataFile*> data;
    std::map<std::string, std::map<std::string, IndexFile*> > indexes;

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
    std::vector<Set> pSets;

    const std::string root;
    std::string name;

    Database(std::string root = "/home/teon/Documents/Database");

    void delete_path(const char* path);

    void _test();

    bool databaseAvailable();

    DataFile* getDataFile(std::string name);

    void selectOneTable(bool all);

    void selectMultiTable(bool all);

    void processSets(UpdateInfo& ui, RecordInfo* ri, std::string tbname);

    void printOneTableSelectResult(SelectResult& sr, std::vector<bool>& selected, RecordInfo* ri);

public:
    static Database* getInstance();

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

    void addPendingSet(Set& set_);

    void insert(std::string name);

    void remove(std::string name);

    void update(std::string name);

    void select(bool all);

    static void test();
};

#endif // DATABASE_H_INCLUDED


