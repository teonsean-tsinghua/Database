#ifndef NODEPAGE_H_INCLUDED
#define NODEPAGE_H_INCLUDED

#include"Page.h"

template<typename T>
class Entry
{
public:
    T key;
    int value;
};

template<typename T>
class NodePage: public Page
{
protected:
    int keyType;
    int keyLength;
    Entry<T> entries[];

public:

    static void split(NodePage* src, NodePage* dest);

    static void merge(NodePage* src, NodePage* dest);

    void insert(T& key, int value);

    void remove(T& key, int value);

    void update(T& key, int oldv, int newv);

    int search(T& key);

    bool isLeaf();

    int getParent();

    void setParent(int parent);

    int getChildCnt();

    void setChildCnt(int cnt);

    const static int PARENT_NODE_OFFSET = PAGE_CONTENT_OFFSET;
    const static int CHILDREN_COUNT_OFFSET = PARENT_NODE_OFFSET + sizeof(int);
    const static int DATA_OFFSET = CHILDREN_COUNT_OFFSET + sizeof(int);

};

#endif // NODEPAGE_H_INCLUDED
