#ifndef INDEXNODEPAGE_H_INCLUDED
#define INDEXNODEPAGE_H_INCLUDED

#include"Page.h"
#include"Slot.h"

class IndexNodePage: public Page
{
protected:
    class IndexNodeSlot: public Slot
    {
    public:
        IndexNodeSlot(BufType cache);
    };

    static BufType buffer;
    IndexNodeSlot* ins;
    int keyType;
    int keyLength;

public:
    IndexNodePage(BufType cache, int index, int pageID, int type, bool parse, int keyType = -1, int keyLength = -1);

    static void split(IndexNodePage* src, IndexNodePage* dest);

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

class IndexLeafPage: public IndexNodePage
{

public:
    IndexLeafPage(BufType cache, int index, int pageID, int mode, int keyType = -1, int keyLength = -1);
};

class IndexInternalPage: public IndexNodePage
{

public:
    IndexInternalPage(BufType cache, int index, int pageID, int mode, int keyType = -1, int keyLength = -1);
};

#endif // INDEXNODEPAGE_H_INCLUDED
