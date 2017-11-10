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
    DBSlot(BufType cache);

    virtual int size();

    virtual void print();

    BufType operator[](const int offset) const;
};

#endif // DBSLOT_H_INCLUDED
