#ifndef DBPAGE_H_INCLUDED
#define DBPAGE_H_INCLUDED

#include"DBPageInfoSlot.h"
#include"../DBInclude.h"

class DBPage
{
protected:
    DBPageInfoSlot* pis;
    BufType cache;
    char* boundary;
    int index;
    int pageID;

    const static char* const name_[];
;
public:
    DBPage(BufType cache, int index, int pageID, int type, int mode);

    BufType operator[](const int offset) const;

    int getIndex();

    int getPageID();

    int getPageType();

    void setNextSameType(int pid);

    int getNextSameType();

    const char* name(int type);

    virtual void print();

    const static int PAGE_INFO_SLOT_OFFSET = 0;

};

#endif // DBPAGE_H_INCLUDED
