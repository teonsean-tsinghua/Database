#ifndef SLOT_H_INCLUDED
#define SLOT_H_INCLUDED

#include"Include.h"

class Slot
{
protected:
    char* cache;

public:
    Slot(char* cache);

    char* operator[](const int offset) const;
};

#endif // SLOT_H_INCLUDED
