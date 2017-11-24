#include "DBIndexDataSlot.h"

DBIndexDataSlot::DBIndexDataSlot(BufType cache, int _dataLen):
    DBSlot(cache)
{
	this -> dataLen = _dataLen;
	fatherPageId = (*this)[FATHER_PAGE_ID_OFFSET];
	isLeaf = (*this)[IS_LEAF_OFFSET];
	dataCnt = (*this)[DATA_CNT_OFFSET];
	pageType = (*this)[PAGE_TYPE_OFFSET];
};

void DBIndexDataSlot::writeData(int idx, char* data, int len){
	char* dataIterator = (char*)(cache) + idx * (dataLen + 2 * sizeof(unsigned int)) + 2 * sizeof(unsigned int) + size();
	for(int i = 0; i < len; i++){
		dataIterator[i] = data[i];
	}
}

void DBIndexDataSlot::writePointer(int idx, unsigned int pagenum){
	writeUnsignedInt((BufType)((char*)cache + size() + idx * (dataLen + 2 * sizeof(unsigned int))), pagenum);
}

void DBIndexDataSlot::writeOffset(int idx, int offset){
	writeInt((BufType)((char*)cache + size() + idx * (dataLen + 2 * sizeof(unsigned int)) + sizeof(unsigned int)), offset);
}

int DBIndexDataSlot::size(){
	return sizeof(unsigned int) * 3 + sizeof(unsigned int);
}

int DBIndexDataSlot::getFatherPageID(){
	int re;
	readInt(fatherPageId, &re);
	return re;
}

void DBIndexDataSlot::setFatherPageID(int _newid){
	writeInt(fatherPageId, _newid);
}

void DBIndexDataSlot::setDataCnt(int _cnt){
	writeInt(dataCnt, _cnt);
}

int DBIndexDataSlot::getIsLeaf(){
	int re;
	readInt(isLeaf, &re);
	return re;
}

void DBIndexDataSlot::setIsLeaf(bool _isLeaf){
	writeInt(isLeaf, (unsigned int)_isLeaf);
}

int DBIndexDataSlot::getDataCnt(){
	int re;
	readInt(dataCnt, &re);
	return re;
}

void DBIndexDataSlot::print(){
	std::cout << "Data Length: " << dataLen << endl;
	std::cout << "Father Page ID: " << getFatherPageID() << endl;
	std::cout << "Data Count: " << getDataCnt() << endl;
	if (getIsLeaf() == 1)
		std::cout << "Leaf Page: " << "true" << endl;
	else
		std::cout << "Leaf Page: " << "false" << endl;
	std::cout << "Page Type: " << getPageType() << endl;
	for(int i = 0; i < getDataCnt(); i++){
		std::cout << "Linker " << i << ", " << "point to page " << getPointerByIdx(i) << ", offset " << getOffsetByIdx(i);
		std::cout << " and key is: ";
		for(int j = 0; j < dataLen; j++){
			printf("%02x", ((char*)cache + size() + 2 * sizeof(unsigned int) * (i + 1) + dataLen * i + j)[0]);
		}
		printf("\n");
	}
}

int DBIndexDataSlot::getPageType(){
	int re;
	readInt(pageType, &re);
	return re;
}

void DBIndexDataSlot::setPageType(int type){
	writeInt(pageType, type);
}

int DBIndexDataSlot::getDataSize(){
	return getDataCnt() * (dataLen + 2 * sizeof(unsigned int));
}

char* DBIndexDataSlot::getDataByIdx(int idx){
	char* re;
	re = (char*)cache + size() + idx * (dataLen + 2 * sizeof(unsigned int)) + 2 * sizeof(unsigned int);
	return re;
}

unsigned int DBIndexDataSlot::getPointerByIdx(int idx){
	unsigned int re;
	readUnsignedInt((*this)[size() + idx * (dataLen + 2 * sizeof(unsigned int))], &re);
	return re;
}

int DBIndexDataSlot::getOffsetByIdx(int idx){
	int re;
	readInt((*this)[size() + idx * (dataLen + 2 * sizeof(unsigned int)) + sizeof(unsigned int)], &re);
	return re;
}

int DBIndexDataSlot::getMaxSize(){
	return (PAGE_SIZE - this -> size()) / (dataLen + 2 * sizeof(unsigned int));
}

void DBIndexDataSlot::appendData(char* data, int size){
	int totsize = getDataSize() + this -> size();
	char* byteCache = (char*) cache;
	for(int i = 0; i < size + 2 * sizeof(unsigned int); i++){
		byteCache[totsize + i] = data[i];
	}
	writeInt(dataCnt, getDataCnt() + 1);
}