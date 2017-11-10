#ifndef DBSLOT_H_INCLUDED
#define DBSLOT_H_INCLUDED

#include"DBFileIOModel.h"
#include"DBCacheIOModel.h"
#include"../DBInclude.h"
#include"DBType.h"

class DBSlot
{
protected:
    BufType cache;

public:
    DBSlot(BufType cache, int mode);

    virtual int size();

    virtual void print();

    BufType operator[](const int offset) const;

    const static int SLOT_LENGTH_OFFSET = 0;    //If slot length is -1, then DBSlot will go find its length at this offset.
};

#endif // DBSLOT_H_INCLUDED
