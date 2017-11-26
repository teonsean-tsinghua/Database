#ifndef DBINDEXFILEDESCRIPTIONSLOT_H_INCLUDED
#define DBINDEXFILEDESCRIPTIONSLOT_H_INCLUDED

#include"DBSlot.h"

/*
 * If modified, you should also rewrite constructor, write(), print(), size(), and add methods as needed.
 */

class DBIndexFileDescriptionSlot: public DBSlot
{
protected:
    BufType firstLeafPage;     // int
    BufType pageNumber;     // int
    BufType rootPage;   // int
    BufType keyType;    // int

public:
    DBIndexFileDescriptionSlot(BufType cache, int mode);

    int getFirstLeafPage();

    int getPageNumber();

    int getRootPage();

    int getKeyType();

    void setFirstLeafPage(int n);

    void setPageNumber(int n);

    void setRootPage(int n);

    void setKeyType(int n);

    void print();

    int size();

    const static int FIRST_LEAF_PAGE_OFFSET = 0;
    const static int PAGE_NUMBER_OFFSET = FIRST_LEAF_PAGE_OFFSET + sizeof(int);
    const static int ROOT_PAGE_OFFSET = PAGE_NUMBER_OFFSET + sizeof(int);
    const static int KEY_TYPE_OFFSET = ROOT_PAGE_OFFSET + sizeof(int);
};

#endif // DBINDEXFILEDESCRIPTIONSLOT_H_INCLUDED
