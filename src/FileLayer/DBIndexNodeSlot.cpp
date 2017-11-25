#include "DBIndexNodeSlot.h"

BufType DBIndexNodeSlot::buffer = (BufType)(new char[PAGE_SIZE]);

DBIndexNodeSlot::DBIndexNodeSlot(BufType cache, int keyType):
    DBSlot(cache), keyType(keyType)
{
	parentNode = (*this)[PARENT_NODE_OFFSET];
	childrenCount = (*this)[CHILDREN_COUNT_OFFSET];
	data = (*this)[DATA_OFFSET];
	keyLength = DBType::typeSize(keyType);
};

int DBIndexNodeSlot::size(){
	return sizeof(int) * 2 + (keyLength + sizeof(int)) * getChildrenCount();
}

int DBIndexNodeSlot::getParentNode(){
	int re;
	readInt(parentNode, &re);
	return re;
}

void DBIndexNodeSlot::setParentNode(int pid){
	writeInt(parentNode, pid);
}

int DBIndexNodeSlot::getChildrenCount(){
	int re;
	readInt(childrenCount, &re);
	return re;
}

void DBIndexNodeSlot::setChildrenCount(int n){
	writeInt(childrenCount, n);
}

void DBIndexNodeSlot::print(){
    DBPrint("Parent node is at ");
    DBPrintLine(getParentNode());
    DBPrint("This node has children of number ");
    DBPrintLine(getChildrenCount());
    for(int i = 0; i < getChildrenCount(); i++)
    {
        BufType key = getKeyOfIndex(i);
        int page = getPageOfIndex(i);
        DBLog("Key: ");
        int re;
        readInt(key, &re);
        DBLog(re);
        DBLog(", page: ");
        DBLogLine(page);
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
    if(larger(key, getMaxKey(), keyType))
    {
        return LARGEST_KEY;
    }
    int cnt = getChildrenCount(), i;
    for(i = 0; i < cnt; i++)
    {
        if(!larger(key, getKeyOfIndex(i), keyType))
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
        if(equal(key, getKeyOfIndex(i), keyType))
        {
            return getPageOfIndex(i);
        }
    }
    return NO_EQUAL_KEY;
}

int DBIndexNodeSlot::insert(void* key, int pid)
{
    int cnt = getChildrenCount();
    if(cnt < 0)
    {
        return ERROR;
    }
    if(cnt == 0 || larger(key, getMaxKey(), keyType))
    {
        setPageOfIndex(cnt, pid);
        setKeyOfIndex(cnt, key);
        setChildrenCount(cnt + 1);
        return SUCCEED;
    }
    int i;
    for(i = 0; i < cnt; i++)
    {
        if(!larger(key, getKeyOfIndex(i), keyType))
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

int DBIndexNodeSlot::update(void* key, int pid)
{
    if(getChildrenCount() <= 0)
    {
        return ERROR;
    }
    int cnt = getChildrenCount();
    for(int i = 0; i < cnt; i++)
    {
        if(equal(key, getKeyOfIndex(i), keyType))
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

