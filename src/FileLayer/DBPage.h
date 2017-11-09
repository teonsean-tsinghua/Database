#ifndef DBPAGE_H_INCLUDED
#define DBPAGE_H_INCLUDED

#include"DBSlot.h"

class DBPage
{
protected:
    DBPageInfoSlot* info;
    DBSlot** slots;
    BufType cache;

public:
    DBPage(BufType cache);

    BufType operator[](const int offset) const;
};

class DBDataPage: protected DBPage
{
};

#endif // DBPAGE_H_INCLUDED
