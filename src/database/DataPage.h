#ifndef DATAPAGE_H_INCLUDED
#define DATAPAGE_H_INCLUDED

#include"Include.h"
#include"Page.h"
#include"RecordSlot.h"

class DataPage: public Page
{
protected:
    std::vector<RecordSlot*> records;
    RecordInfo* ri;

public:
    DataPage(char* cache, int index, int pageID, bool parse, RecordInfo* ri);

    int insert(std::vector<void*>& data);

    void read(int i, std::vector<void*>& data);

    void print();

    int remove(SearchInfo& si);

    void select(SearchInfo& si, SelectResult& sr);

    int update(SearchInfo& si, UpdateInfo& ui);

    const static int SUCCEED = 0;
    const static int PAGE_FULL = 1;

};

#endif // DATAPAGE_H_INCLUDED
