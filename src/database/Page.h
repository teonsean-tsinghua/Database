#ifndef PAGE_H_INCLUDED
#define PAGE_H_INCLUDED

#include"PageInfoSlot.h"
#include"Include.h"

class Page
{
protected:
    PageInfoSlot* pis;
    BufType cache;
    char* boundary;
    int index;
    int pageID;

    const static char* const name_[];

public:
    Page(BufType cache, int index, int pageID, int type);

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

#endif // PAGE_H_INCLUDED
