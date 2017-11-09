#ifndef DBSLOT_H_INCLUDED
#define DBSLOT_H_INCLUDED

#include"DBFileIOModel.h"
#include"DBParser.h"
#include"../DBInclude.h"

class DBSlot
{
protected:
    BufType cache;
    int slotLength;

public:
    DBSlot(BufType cache, int slotLength = 0, int parse = false);

    virtual int size();

    virtual void print();

    int getSlotLength();

    BufType operator[](const int offset) const;

    const static int SLOT_LENGTH_OFFSET = 0;    //If slot length is -1, then DBSlot will go find its length at this offset.
};

#endif // DBSLOT_H_INCLUDED
