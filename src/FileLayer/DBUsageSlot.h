#ifndef DBUSAGESLOT_H_INCLUDED
#define DBUSAGESLOT_H_INCLUDED

#include"DBSlot.h"

class DBUsageSlot:public DBSlot
{
protected:
    void setAvailable(int offset, bool available);

    bool isAvailable(int offset);
};

#endif // DBUSAGESLOT_H_INCLUDED
