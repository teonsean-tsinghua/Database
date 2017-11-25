#ifndef DBINDEXNODESLOT_H_INCLUDED
#define DBINDEXNODESLOT_H_INCLUDED

#include "DBSlot.h"

class DBIndexNodeSlot: public DBSlot
{
private:
    static BufType buffer;
	int keyLength;
	int keyType;
	int maxSize;
	BufType parentNode;
	BufType childrenCount;
	BufType data;

public:
    DBIndexNodeSlot(BufType cache, int keyType);

    int size();

    void print();

    int getParentNode();

    void setParentNode(int pid);

    int getChildrenCount();

    void setChildrenCount(int n);

    BufType getMinKey();

    BufType getMaxKey();

    int getPageOfIndex(int index);

    void setPageOfIndex(int index, int pid);

    BufType getKeyOfIndex(int index);

    void setKeyOfIndex(int index, void* key);

    int search(void* key);

    int searchEqual(void* key);

    int insert(void* key, int pid);

    const static int PARENT_NODE_OFFSET = 0;
    const static int CHILDREN_COUNT_OFFSET = PARENT_NODE_OFFSET + sizeof(int);
    const static int DATA_OFFSET = CHILDREN_COUNT_OFFSET + sizeof(int);
};

#endif // DBINDEXNODESLOT_H_INCLUDED
