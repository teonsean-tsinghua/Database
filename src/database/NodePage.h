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
    char* buffer;

public:
    NodePage(char* cache, int index, int pageID, int keyType, int keyLength, bool parse, int type);

    ~NodePage();

    void print();

    bool greaterThanAll(T& key);

    void insert(T& key, int value);

    int remove(T& key, int value = 0);

    void update(T& key, int newv, int oldv = 0);

    Entry<T>* at(int i);

    Entry<T>* search(T& key);

    int searchForIndex(T& key);

    int getKeyLength();

    bool isLeaf();

    bool isFull();

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

template<typename T>
LeafPage<T>::LeafPage(char* cache, int index, int pageID, int keyType, int keyLength, bool parse):
    NodePage<T>(cache, index, pageID, keyType, keyLength, parse, Type::LEAF_PAGE)
{
    if(parse)
    {
        assert(Page::getPageType() == Type::LEAF_PAGE);
    }
}

template<typename T>
InternalPage<T>::InternalPage(char* cache, int index, int pageID, int keyType, int keyLength, bool parse):
    NodePage<T>(cache, index, pageID, keyType, keyLength, parse, Type::INTERNAL_PAGE)
{
    if(parse)
    {
        assert(Page::getPageType() == Type::INTERNAL_PAGE);
    }
}

template<typename T>
NodePage<T>::NodePage(char* cache, int index, int pageID, int keyType, int keyLength, bool parse, int type):
    Page(cache, index, pageID), keyType(keyType), keyLength(keyLength)
{
    if(!parse)
    {
        setFirstAvailableByte(PAGE_CONTENT_OFFSET);
        setPrevSamePage(-1);
        setNextSamePage(-1);
        setChildCnt(0);
        setParent(-1);
        setPageType(type);
    }
    buffer = new char[8192];
}

template<typename T>
NodePage<T>::~NodePage()
{
    delete buffer;
}

template<typename T>
void NodePage<T>::print()
{
    Page::print();
    std::cout << "Parent id:                        " << getParent() << std::endl;
    for(int i = 0; i < getChildCnt(); i++)
    {
        std::cout << "idx " << i << " key " << at(i)->key << " value " << at(i)->value << "\n";
    }
}

template<typename T>
int NodePage<T>::getChildCnt()
{
	return readInt((*this)[CHILDREN_COUNT_OFFSET]);
}

template<typename T>
int NodePage<T>::getKeyLength()
{
	return keyLength;
}

template<typename T>
void NodePage<T>::setChildCnt(int n)
{
	writeInt((*this)[CHILDREN_COUNT_OFFSET], n);
}

template<typename T>
bool NodePage<T>::isFull()
{
    return (getFirstAvailableByte() + sizeof(int) + keyLength) > PAGE_SIZE;
}

template<typename T>
bool NodePage<T>::isLeaf()
{
    return getPageType() == Type::LEAF_PAGE;
}

template<typename T>
Entry<T>* NodePage<T>::at(int n)
{
    return (Entry<T>*)((*this)[DATA_OFFSET + n * (keyLength + sizeof(int))]);
}

template<typename T>
int NodePage<T>::searchForIndex(T& key)
{
    int cnt = getChildCnt();
    assert(cnt >= 0);
    for(int i = 0; i < cnt; i++)
    {
        if(key <= at(i)->key)
        {
            return i;
        }
    }
    return cnt;
}

template<typename T>
Entry<T>* NodePage<T>::search(T& key)
{
    int cnt = getChildCnt();
    assert(cnt >= 0);
    for(int i = 0; i < cnt; i++)
    {
        if(key <= at(i)->key)
        {
            return at(i);
        }
    }
    return NULL;
}

template<typename T>
void NodePage<T>::update(T& key, int newv, int oldv)
{
    int cnt = getChildCnt();
    assert(cnt >= 0);
    for(int i = 0; i < cnt; i++)
    {
        if(key == at(i)->key)
        {
            assert(oldv <= 0 || oldv == at(i)->value);
            at(i)->value = newv;
            return;
        }
    }
}

template<typename T>
int NodePage<T>::remove(T& key, int value)
{
    int cnt = getChildCnt();
    assert(cnt >= 0);
    for(int i = 0; i < cnt; i++)
    {
        if(key == at(i)->key)
        {
            assert(value <= 0 || value == at(i)->value);
            int len = (cnt - i - 1) * (sizeof(int) + keyLength);
            copyData((char*)&(at(i + 1)->value), buffer, len);
            copyData(buffer, (char*)&(at(i)->value), len);
            setChildCnt(cnt - 1);
            setFirstAvailableByte(getFirstAvailableByte() - keyLength - sizeof(int));
            return i;
        }
    }
    return -1;
}

template<typename T>
void NodePage<T>::insert(T& key, int value)
{
    int cnt = getChildCnt();
    assert(cnt >= 0);
    int idx;
    for(idx = 0; idx < cnt; idx++)
    {
        if(key <= at(idx)->key)
        {
            break;
        }
    }
    if(idx != cnt)
    {
        assert(idx < cnt);
        assert(key != at(idx)->key);
    }
    int len = (cnt - idx) * (sizeof(int) + keyLength);
    copyData((char*)&(at(idx)->value), buffer, len);
    copyData(buffer, (char*)&(at(idx + 1)->value), len);
    at(idx)->key << key;
    at(idx)->value = value;
    setChildCnt(cnt + 1);
    setFirstAvailableByte(getFirstAvailableByte() + keyLength + sizeof(int));
}

template<typename T>
void NodePage<T>::setParent(int pid)
{
    writeInt((*this)[PARENT_NODE_OFFSET], pid);
}

template<typename T>
int NodePage<T>::getParent()
{
	return readInt((*this)[PARENT_NODE_OFFSET]);
}

template<typename T>
bool NodePage<T>::greaterThanAll(T& key)
{
    int cnt = getChildCnt();
    assert(cnt >= 0);
    return cnt == 0 || key > at(cnt - 1)->key;
}

template<typename T>
void NodePage<T>::test()
{
    char *p1;
    p1 = new char[8192];
    int *nums;
    nums = new int[10];
    LeafPage<IntType>* lp = new LeafPage<IntType>(p1, 1, 2, Type::INT, 4, false);
    nums[0] = 789;
    nums[1] = 987;
    nums[2] = 897;
    nums[3] = 0;
    lp->insert(*(IntType*)nums, 0);
    lp->insert(*(IntType*)(nums + 1), 1);
    lp->insert(*(IntType*)(nums + 2), 2);
    lp->insert(*(IntType*)(nums + 3), 3);
    assert(lp->at(0)->value == 3);
    assert(lp->at(1)->value == 0);
    assert(lp->at(2)->value == 2);
    assert(lp->at(3)->value == 1);
    assert(lp->at(0)->key.v == 0);
    assert(lp->at(1)->key.v == 789);
    assert(lp->at(2)->key.v == 897);
    assert(lp->at(3)->key.v == 987);
    lp->update(*(IntType*)(nums + 2), 222, 2);
    assert(lp->at(0)->value == 3);
    assert(lp->at(1)->value == 0);
    assert(lp->at(2)->value == 222);
    assert(lp->at(3)->value == 1);
    assert(lp->at(0)->key.v == 0);
    assert(lp->at(1)->key.v == 789);
    assert(lp->at(2)->key.v == 897);
    assert(lp->at(3)->key.v == 987);
    LeafPage<IntType>* lp2 = new LeafPage<IntType>(p1, 1, 2, Type::INT, 4, true);
    Entry<IntType>* en = lp2->search(*(IntType*)(nums + 1));
    assert(en->key.v == 987);
    assert(en->value == 1);
    lp2->remove(*(IntType*)nums);
    assert(lp->at(0)->value == 3);
    assert(lp->at(1)->value == 222);
    assert(lp->at(2)->value == 1);
    assert(lp->at(0)->key.v == 0);
    assert(lp->at(1)->key.v == 897);
    assert(lp->at(2)->key.v == 987);
    std::cout << "Passed node page tests.\n";
}

#endif // NODEPAGE_H_INCLUDED
