#ifndef DBPAGE_H_INCLUDED
#define DBPAGE_H_INCLUDED

#include"DBSlot.h"

class DBPage
{
protected:
    DBPageInfoSlot* info;
    BufType cache;
};

class DBDataPage: protected DBPage
{
protected:

};

#endif // DBPAGE_H_INCLUDED
