#ifndef DBINDEXNODEPAGE_H_INCLUDED
#define DBINDEXNODEPAGE_H_INCLUDED

#include"DBPage.h"
#include"DBIndexNodeSlot.h"

class DBIndexNodePage: public DBPage
{
protected:
    DBIndexNodeSlot* ins;
    int keyType;
    int keyLength;

public:
    DBIndexNodePage(BufType cache, int index, int pageID, int type, int mode, int keyType = -1, int keyLength = -1);

    static void split(DBIndexNodePage* src, DBIndexNodePage* dest);

    void calcDegree(int& minDgr, int& maxDgr);

    void setParent(int pid);

    int getParent();

    void changeKeyOfPage(int page, void* key);

    int search(void* key);

    int searchEqual(void* key);

    void insert(void* key, int pid);

    void remove(void* key);

    void* getMaxKey();

    void setMaxKey(void* key);

    int getChildrenCount();

    int getMinPage();

    int getMaxPage();

    void update(void* key, int pid);

    void print();

    bool isLeaf();
};

#endif // DBINDEXNODEPAGE_H_INCLUDED
