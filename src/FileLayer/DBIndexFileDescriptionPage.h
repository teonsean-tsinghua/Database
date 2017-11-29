#ifndef DBINDEXFILEDESCRIPTIONPAGE_H_INCLUDED
#define DBINDEXFILEDESCRIPTIONPAGE_H_INCLUDED

#include"DBPage.h"
#include"DBIndexFileDescriptionSlot.h"

class DBIndexFileDescriptionPage: public DBPage
{
protected:
    DBIndexFileDescriptionSlot* ifds;

public:
    DBIndexFileDescriptionPage(BufType cache, int index, int pageID, int mode, int type = -1, int length = -1);

    void incrementPageNumber();

    void setFirstLeafPage(int pid);

    void setRootPage(int pid);

    int getPageNumber();

    int getKeyLength();

    int getKeyType();

    int getFirstLeafPage();

    int getRootPage();

    void print();
};

#endif // DBINDEXFILEDESCRIPTIONPAGE_H_INCLUDED
