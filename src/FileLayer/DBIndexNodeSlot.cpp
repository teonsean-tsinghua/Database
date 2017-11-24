#include "DBIndexNodeSlot.h"

DBIndexNodeSlot::DBIndexNodeSlot(BufType cache, int keyLength):
    DBSlot(cache), keyLength(keyLength)
{
	parentNode = (*this)[PARENT_NODE_OFFSET];
	childrenCount = (*this)[CHILDREN_COUNT_OFFSET];
	data = (*this)[DATA_OFFSET];
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
