#ifndef DBINDEXNODEPAGE_H_INCLUDED
#define DBINDEXNODEPAGE_H_INCLUDED

#include"DBPage.h"
#include"DBIndexNodeSlot.h"

class DBIndexNodePage: public DBPage
{
protected:
    DBIndexNodeSlot* ins;
    int keyLength;
    int keyType;

public:
    DBIndexNodePage(BufType cache, int index, int pageID, int type, int mode, int keyType = -1);

    void calcDegree(int& minDgr, int& maxDgr);

    void setParent(int pid);

    int search(void* key);

    int insert(void* key, int pid);

    void* getMaxKey();

    int getMaxPage();

    void print();

    bool isLeaf();
};

#endif // DBINDEXNODEPAGE_H_INCLUDED
