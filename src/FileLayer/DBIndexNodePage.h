#ifndef DBINDEXNODEPAGE_H_INCLUDED
#define DBINDEXNODEPAGE_H_INCLUDED

#include"DBPage.h"
#include"DBSlot.h"

class DBIndexNodePage: public DBPage
{
protected:
    class DBIndexNodeSlot: public DBSlot
    {
    public:
        DBIndexNodeSlot(BufType cache);
    };

    static BufType buffer;
    DBIndexNodeSlot* ins;
    int keyType;
    int keyLength;

public:
    DBIndexNodePage(BufType cache, int index, int pageID, int type, int mode, int keyType = -1, int keyLength = -1);

    static void split(DBIndexNodePage* src, DBIndexNodePage* dest);

    int getPageOfIndex(int index);

    void setPageOfIndex(int index, int pid);

    BufType getKeyOfIndex(int index);

    void setKeyOfIndex(int index, void* key);

    void calcDegree(int& minDgr, int& maxDgr);

    void setParentNode(int pid);

    int getParentNode();

    void changeKeyOfPage(int page, void* key);

    int search(void* key);

    int searchEqual(void* key);

    void insert(void* key, int pid);

    void remove(void* key);

    void* getMaxKey();

    void* getMinKey();

    void setMaxKey(void* key);

    int getChildrenCount();

    void setChildrenCount(int n);

    int getMinPage();

    int getMaxPage();

    void update(void* key, int pid);

    void print();

    bool isLeaf();

    void updateFirstAvailable();

    const static int PARENT_NODE_OFFSET = 0;
    const static int CHILDREN_COUNT_OFFSET = PARENT_NODE_OFFSET + sizeof(int);
    const static int DATA_OFFSET = CHILDREN_COUNT_OFFSET + sizeof(int);
};

class DBIndexLeafPage: public DBIndexNodePage
{

public:
    DBIndexLeafPage(BufType cache, int index, int pageID, int mode, int keyType = -1, int keyLength = -1);
};

class DBIndexInternalPage: public DBIndexNodePage
{

public:
    DBIndexInternalPage(BufType cache, int index, int pageID, int mode, int keyType = -1, int keyLength = -1);
};

#endif // DBINDEXNODEPAGE_H_INCLUDED
