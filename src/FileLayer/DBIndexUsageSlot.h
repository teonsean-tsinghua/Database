#ifndef DBINDEXUSAGESLOT_H_INCLUDED
#define DBINDEXUSAGESLOT_H_INCLUDED

#include"DBSlot.h"

class DBIndexUsageSlot:public DBSlot
{
public:
    DBIndexUsageSlot(BufType cache);

    void setAvailable(int offset, bool available);

    bool isAvailable(int offset);
};

#endif // DBUSAGESLOT_H_INCLUDED
