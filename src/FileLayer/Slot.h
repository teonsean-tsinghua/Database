#ifndef SLOT_H_INCLUDED
#define SLOT_H_INCLUDED

#include"FileIOModel.h"
#include"CacheIOModel.h"
#include"Include.h"

class Slot
{
protected:
    BufType cache;

public:
    Slot(BufType cache);

    BufType operator[](const int offset) const;
};

#endif // SLOT_H_INCLUDED
