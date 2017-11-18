#ifndef DBINDEXUSAGESLOT_H_INCLUDED
#define DBINDEXUSAGESLOT_H_INCLUDED

class DBIndexUsageSlot:public DBSlot
{
public:
    DBIndexUsageSlot(BufType cache);

    void setAvailable(int offset, bool available);

    bool isAvailable(int offset);
};

#endif