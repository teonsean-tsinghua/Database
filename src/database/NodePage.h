#ifndef NODEPAGE_H_INCLUDED
#define NODEPAGE_H_INCLUDED

#include"Page.h"

template<typename T>
class Entry
{
public:
    int value;
    T key;
};

template<typename T>
class NodePage: public Page
{
protected:
    int keyType;
    int keyLength;
    Entry<T> entries[];

public:
    NodePage(char* cache, int index, int pageID, int keyType, int keyLength, bool parse, int type);

    bool greaterThanAll(T& key);

    void insert(T& key, int value);

    void remove(T& key, int value = 0);

    void update(T& key, int newv, int oldv = 0);

    Entry<T>* at(int i);

    Entry<T>* search(T& key);

    bool isLeaf();

    int getParent();

    void setParent(int parent);

    int getChildCnt();

    void setChildCnt(int cnt);

    static void test();

    const static int PARENT_NODE_OFFSET = PAGE_CONTENT_OFFSET;
    const static int CHILDREN_COUNT_OFFSET = PARENT_NODE_OFFSET + sizeof(int);
    const static int DATA_OFFSET = CHILDREN_COUNT_OFFSET + sizeof(int);

};

template<typename T>
class InternalPage: public NodePage<T>
{
public:
    InternalPage(char* cache, int index, int pageID, int keyType, int keyLength, bool parse);
};

template<typename T>
class LeafPage: public NodePage<T>
{
public:
    LeafPage(char* cache, int index, int pageID, int keyType, int keyLength, bool parse);
};

#endif // NODEPAGE_H_INCLUDED
