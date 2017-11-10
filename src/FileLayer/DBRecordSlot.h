#ifndef DBRECORDSLOT_H_INCLUDED
#define DBRECORDSLOT_H_INCLUDED

#include"DBSlot.h"

class DBRecordSlot: public DBSlot
{
protected:
    DBRecordInfo* ri;

public:
    DBRecordSlot(BufType cache, DBRecordInfo* ri);

    int write(std::vector<void*>& data);

    int read(std::map<int, void*>& data);

    void print();
};

#endif // DBRECORDSLOT_H_INCLUDED
