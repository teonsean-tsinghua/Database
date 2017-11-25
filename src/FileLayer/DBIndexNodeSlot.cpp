#include "DBIndexNodeSlot.h"

char* DBIndexNodeSlot::buffer = new char[PAGE_SIZE];

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
//    DBLogLine("Before");
//    DBLogLine(*(int*)(*this)[DATA_OFFSET + index * (keyLength + sizeof(int)) + keyLength]);
    writeInt((*this)[DATA_OFFSET + index * (keyLength + sizeof(int)) + keyLength], pid);
//    DBLogLine("After");
//    DBLogLine(*(int*)(*this)[DATA_OFFSET + index * (keyLength + sizeof(int)) + keyLength]);
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
    for(int j = cnt - 1; j >= i; j--)
    {
        setKeyOfIndex(j + 1, getKeyOfIndex(j));
        setPageOfIndex(j + 1, getPageOfIndex(j));
    }
    setKeyOfIndex(i, key);
    setPageOfIndex(i, pid);
    setChildrenCount(cnt + 1);
    return SUCCEED;
}

BufType DBIndexNodeSlot::getMinKey()
{
    return (*this)[DATA_OFFSET];
}

BufType DBIndexNodeSlot::getMaxKey()
{
    return (*this)[DATA_OFFSET + (sizeof(int) + keyLength) * (getChildrenCount() - 1)];
}

//void DBIndexNodeSlot::writeData(int idx, char* data, int len){
//	char* dataIterator = (char*)(cache) + idx * (dataLen + 2 * sizeof(unsigned int)) + 2 * sizeof(unsigned int) + size();
//	for(int i = 0; i < len; i++){
//		dataIterator[i] = data[i];
//	}
//}
//
//void DBIndexNodeSlot::writePointer(int idx, unsigned int pagenum){
//	writeUnsignedInt((BufType)((char*)cache + size() + idx * (dataLen + 2 * sizeof(unsigned int))), pagenum);
//}
//
//void DBIndexNodeSlot::writeOffset(int idx, int offset){
//	writeInt((BufType)((char*)cache + size() + idx * (dataLen + 2 * sizeof(unsigned int)) + sizeof(unsigned int)), offset);
//}
//
//char* DBIndexNodeSlot::getDataByIdx(int idx){
//	char* re;
//	re = (char*)cache + size() + idx * (dataLen + 2 * sizeof(unsigned int)) + 2 * sizeof(unsigned int);
//	return re;
//}
//
//unsigned int DBIndexNodeSlot::getPointerByIdx(int idx){
//	unsigned int re;
//	readUnsignedInt((*this)[size() + idx * (dataLen + 2 * sizeof(unsigned int))], &re);
//	return re;
//}
//
//int DBIndexNodeSlot::getOffsetByIdx(int idx){
//	int re;
//	readInt((*this)[size() + idx * (dataLen + 2 * sizeof(unsigned int)) + sizeof(unsigned int)], &re);
//	return re;
//}
//
//void DBIndexNodeSlot::appendData(char* data, int size){
//	int totsize = getDataSize() + this -> size();
//	char* byteCache = (char*) cache;
//	for(int i = 0; i < size + 2 * sizeof(unsigned int); i++){
//		byteCache[totsize + i] = data[i];
//	}
//	writeInt(dataCnt, getDataCnt() + 1);
//}
