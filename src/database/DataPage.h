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
    int cur;

public:
    DataPage(char* cache, int index, int pageID, bool parse, RecordInfo* ri);

    int insert(std::vector<void*>& data);

    void read(int i, std::vector<void*>& data);

    void print();

    bool isFull();

    int remove(SearchInfo& si);

    void select(SearchInfo& si, SelectResult& sr);

    int update(SearchInfo& si, UpdateInfo& ui);

    void initIterator();

    char* getNext(int fidx, int& ridx);

};

#endif // DATAPAGE_H_INCLUDED
