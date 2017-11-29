#ifndef DBDATAPAGE_H_INCLUDED
#define DBDATAPAGE_H_INCLUDED

#include"../DBInclude.h"
#include"DBPage.h"
#include"DBRecordSlot.h"

class DBDataPage: public DBPage
{
protected:
    std::vector<DBRecordSlot*> records;
    DBRecordInfo* ri;

public:
    DBDataPage(BufType cache, int index, int pageID, int mode);

    int insert(std::vector<void*>& data);

    void print();

    void printAllRecords();

    int remove(std::map<int, void*>& data);

    int update(std::map<int, void*>& key_value, std::map<int, void*>& update_value);

    int findEqual(std::map<int, void*>& data, std::set<std::map<std::string, void*>*>& result);

    int findEqual(std::map<int, void*>& data, std::set<char*>& result);

};

#endif // DBDATAPAGE_H_INCLUDED
