#ifndef DATAPAGE_H_INCLUDED
#define DATAPAGE_H_INCLUDED

#include"../Include.h"
#include"Page.h"
#include"RecordSlot.h"

class DataPage: public Page
{
protected:
    std::vector<RecordSlot*> records;
    RecordInfo* ri;

public:
    DataPage(BufType cache, int index, int pageID, bool parse, RecordInfo* ri);

    int insert(std::vector<void*>& data);

    void print();

    void printAllRecords();

    int remove(SearchInfo& si);

    void filterByNull(std::map<int, bool>& nulls, std::list<std::vector<void*> >& datas);

    void filterByValue(std::map<int, std::vector<void*> >& info, std::list<std::vector<void*> >& datas, int op);

    void filterByFields(std::map<int, std::vector<int> >& info, std::list<std::vector<void*> >& datas, int op);

    int update(SearchInfo& si, UpdateInfo& ui);

    const static int SUCCEED = 0;
    const static int PAGE_FULL = 1;

};

#endif // DATAPAGE_H_INCLUDED
