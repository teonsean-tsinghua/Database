#ifndef DBINDEXNODEPAGE_H_INCLUDED
#define DBINDEXNODEPAGE_H_INCLUDED

#include"DBPage.h"
#include"DBIndexNodeSlot.h"

class DBIndexNodePage: public DBPage
{
protected:
    DBIndexNodeSlot* ins;
    int keyLength;

public:
    DBIndexNodePage(BufType cache, int index, int pageID, int type, int mode, int keyLength = -1);

    void calcDegree(int& minDgr, int& maxDgr);

    void setParent(int pid);

    void print();

    bool isLeaf();
};

#endif // DBINDEXNODEPAGE_H_INCLUDED
