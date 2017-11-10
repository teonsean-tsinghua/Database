#ifndef DBRECORDSLOT_H_INCLUDED
#define DBRECORDSLOT_H_INCLUDED

#include"DBSlot.h"

class DBRecordSlot: public DBSlot
{
public:
    DBRecordSlot(BufType cache);
};

#endif // DBRECORDSLOT_H_INCLUDED
