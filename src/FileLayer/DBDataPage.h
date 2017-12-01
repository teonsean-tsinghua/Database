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
    DBDataPage(BufType cache, int index, int pageID, int mode, DBRecordInfo* ri);

    int insert(std::vector<void*>& data);

    void print();

    void printAllRecords();

    void remove(std::map<int, void*>& data);

    void filterByNull(std::map<int, bool>& nulls, std::list<std::vector<void*> >& datas);

    void filterByValue(std::map<int, void*>& info, std::list<std::vector<void*> >& datas, int op);

    void filterByFields(std::map<int, int>& info, std::list<std::vector<void*> >& datas, int op);

    void update(std::map<int, void*>& key_value, std::map<int, void*>& update_value);

    void findEqual(std::map<int, void*>& data, std::set<std::map<std::string, void*>*>& result);

    void findEqual(std::map<int, void*>& data, std::set<char*>& result);

    const static int SUCCEED = 0;
    const static int PAGE_FULL = 1;

};

#endif // DBDATAPAGE_H_INCLUDED
