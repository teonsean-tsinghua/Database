#include"NodePage.h"

static char* buffer = new char[PAGE_SIZE];

template<typename T>
int NodePage<T>::getChildCnt()
{
	return readInt((*this)[CHILDREN_COUNT_OFFSET]);
}

template<typename T>
void NodePage<T>::setChildCnt(int n)
{
	writeInt((*this)[CHILDREN_COUNT_OFFSET], n);
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
        if(key == at(i).key)
        {
            assert(oldv <= 0 || oldv == at(i).value);
            at(i)->value = newv;
            return;
        }
    }
}

template<typename T>
void NodePage<T>::remove(T& key, int value)
{
    int cnt = getChildCnt();
    assert(cnt >= 0);
    for(int i = 0; i < cnt; i++)
    {
        if(key == at(i).key)
        {
            assert(value <= 0 || value == at(i).value);
            int len = (cnt - i - 1) * (sizeof(int) + keyLength);
            copyData(at(i + 1)->value, buffer, len);
            copyData(buffer, at(i)->value, len);
            setChildCnt(cnt - 1);
            setFirstAvailableByte(getFirstAvailableByte() - keyLength - sizeof(int));
            return;
        }
    }
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
    assert(idx == cnt || (idx < cnt && key != at(idx)->key));
    int len = (cnt - idx) * (sizeof(int) + keyLength);
    copyData((char*)&(at(idx)->value), buffer, len);
    copyData(buffer, (char*)&(at(idx + 1)->value), len);
    at(idx)->key << key;
    at(idx).value = value;
    setChildCnt(cnt + 1);
    setFirstAvailableByte(getFirstAvailableByte() + keyLength + sizeof(int));
}

template<typename T>
void NodePage<T>::setParent(int pid)
{
    assert(pid > 0);
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
    return cnt == 0 || key > entries[cnt - 1].key;
}
