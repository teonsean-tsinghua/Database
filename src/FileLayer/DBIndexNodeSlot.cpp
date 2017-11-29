#include "DBIndexNodeSlot.h"

BufType DBIndexNodeSlot::buffer = (BufType)(new char[PAGE_SIZE]);

DBIndexNodeSlot::DBIndexNodeSlot(BufType cache, int keyType, int keyLength):
    DBSlot(cache), keyType(keyType), keyLength(keyLength)
{
};

int DBIndexNodeSlot::size(){
	return sizeof(int) * 2 + (keyLength + sizeof(int)) * getChildrenCount();
}

int DBIndexNodeSlot::getParentNode(){
	int re;
	readInt((*this)[PARENT_NODE_OFFSET], &re);
	return re;
}

void DBIndexNodeSlot::setParentNode(int pid){
	writeInt((*this)[PARENT_NODE_OFFSET], pid);
}

int DBIndexNodeSlot::getChildrenCount(){
	int re;
	readInt((*this)[CHILDREN_COUNT_OFFSET], &re);
	return re;
}

void DBIndexNodeSlot::setChildrenCount(int n){
	writeInt((*this)[CHILDREN_COUNT_OFFSET], n);
}

void DBIndexNodeSlot::print(){
    DBPrint::print("Parent node is at ").printLine(getParentNode())
            .print("This node has children of number ").printLine(getChildrenCount());
    for(int i = 0; i < getChildrenCount(); i++)
    {
        BufType key = getKeyOfIndex(i);
        int page = getPageOfIndex(i);
        int re;
        readInt(key, &re);
        DBPrint::log("Key: ").log(re).log(", page: ").logLine(page);
    }
}

int DBIndexNodeSlot::getPageOfIndex(int index)
{
    int re;
    readInt((*this)[DATA_OFFSET + index * (keyLength + sizeof(int)) + keyLength], &re);
    return re;
}

void DBIndexNodeSlot::setPageOfIndex(int index, int pid)
{
    writeInt((*this)[DATA_OFFSET + index * (keyLength + sizeof(int)) + keyLength], pid);
}

BufType DBIndexNodeSlot::getKeyOfIndex(int index)
{
    return (*this)[DATA_OFFSET + index * (keyLength + sizeof(int))];
}

void DBIndexNodeSlot::setKeyOfIndex(int index, void* key)
{
    writeData((*this)[DATA_OFFSET + index * (keyLength + sizeof(int))], (char*)key, keyLength);
}

int DBIndexNodeSlot::search(void* key)
{
    if(getChildrenCount() <= 0)
    {
        return ERROR;
    }
    if(larger(key, getMaxKey(), keyType, keyLength))
    {
        return LARGEST_KEY;
    }
    int cnt = getChildrenCount(), i;
    for(i = 0; i < cnt; i++)
    {
        if(!larger(key, getKeyOfIndex(i), keyType, keyLength))
        {

            break;
        }
    }
    return getPageOfIndex(i);
}

int DBIndexNodeSlot::searchEqual(void* key)
{
    if(getChildrenCount() <= 0)
    {
        return ERROR;
    }
    int cnt = getChildrenCount();
    for(int i = 0; i < cnt; i++)
    {
        if(equal(key, getKeyOfIndex(i), keyType, keyLength))
        {
            return getPageOfIndex(i);
        }
    }
    return NO_EQUAL_KEY;
}

int DBIndexNodeSlot::insert(void* key, int pid)
{
    if(searchEqual(key) > 0)
    {
        return ERROR;
    }
    int cnt = getChildrenCount();
    if(cnt < 0)
    {
        return ERROR;
    }
    if(cnt == 0 || larger(key, getMaxKey(), keyType, keyLength))
    {
        setPageOfIndex(cnt, pid);
        setKeyOfIndex(cnt, key);
        setChildrenCount(cnt + 1);
        return SUCCEED;
    }
    int i;
    for(i = 0; i < cnt; i++)
    {
        if(!larger(key, getKeyOfIndex(i), keyType, keyLength))
        {
            break;
        }
    }
    int len = (cnt - i) * (sizeof(int) + keyLength);
    copyData(getKeyOfIndex(i), buffer, len);
    copyData(buffer, getKeyOfIndex(i + 1), len);
    setKeyOfIndex(i, key);
    setPageOfIndex(i, pid);
    setChildrenCount(cnt + 1);
    return SUCCEED;
}

int DBIndexNodeSlot::remove(void* key)
{
    if(getChildrenCount() <= 0)
    {
        return ERROR;
    }
    int cnt = getChildrenCount();
    for(int i = 0; i < cnt; i++)
    {
        if(equal(key, getKeyOfIndex(i), keyType, keyLength))
        {
            int len = (cnt - i - 1) * (sizeof(int) + keyLength);
            copyData(getKeyOfIndex(i + 1), buffer, len);
            copyData(buffer, getKeyOfIndex(i), len);
            setChildrenCount(cnt - 1);
            return SUCCEED;
        }
    }
    return NO_EQUAL_KEY;
}

int DBIndexNodeSlot::update(void* key, int pid)
{
    if(getChildrenCount() <= 0)
    {
        return ERROR;
    }
    int cnt = getChildrenCount();
    for(int i = 0; i < cnt; i++)
    {
        if(equal(key, getKeyOfIndex(i), keyType, keyLength))
        {
            setPageOfIndex(i, pid);
            return SUCCEED;
        }
    }
    return NO_EQUAL_KEY;
}

BufType DBIndexNodeSlot::getMinKey()
{
    return (*this)[DATA_OFFSET];
}

BufType DBIndexNodeSlot::getMaxKey()
{
    return (*this)[DATA_OFFSET + (sizeof(int) + keyLength) * (getChildrenCount() - 1)];
}

