#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include"DataFile.h"
#include"y.tab.h"
#include"IndexFile.h"

class Database
{
private:
    static Database* instance;
    const static std::string TAG;

    FileIOModel* fm;

    std::vector<std::string> pNames;
    std::vector<int> pTypes;
    std::vector<int> pNullables;
    std::vector<int> pExtras;
    std::vector<Value> pValues;
    std::vector<std::vector<Value> > pValueLists;
    std::vector<std::string> pTables;
    std::vector<std::string> pColumns;
    std::vector<Col> pCols;
    std::vector<Where> pWheres;
    std::vector<Set> pSets;
    std::vector<Foreign> pForeigns;
    std::string curDb;

    Database(std::string root = "/home/teon/Documents/Database");

    void _test();

    void selectOneTable(bool all);

    void selectMultiTable(bool all);

//    void processSets(UpdateInfo& ui, RecordInfo* ri, std::string tbname);
//
//    void printOneTableSelectResult(SelectResult& sr, std::vector<bool>& selected, RecordInfo* ri);

public:
    static Database* getInstance();

    void init();

    void createDatabase(std::string name);

    void dropDatabase(std::string name);

    void useDatabase(std::string name);

    void showDatabases();

    void createTable(std::string name);

    void dropTable(std::string name);

    void describeTable(std::string name);

    void createIndex(std::string tbname, std::string colname);

    void dropIndex(std::string tbname, std::string colname);

    void showTables();

    void addPendingField(std::string& name, int type, bool nullable, int extra);

    void addPendingTable(std::string& name);

    void addPendingColumn(std::string& name);

    void addPendingCol(Col& col);

    void addPendingValue(Value& value);

    void addPendingValueList();

    void addPendingWhere(Where& where);

    void addPendingSet(Set& set_);

    void addPendingForeign(Foreign& foreign);

    void insert(std::string name);

    void remove(std::string name);

    void update(std::string name);

    void select(bool all);

    static void test();
};

#endif // DATABASE_H_INCLUDED


