#include "DBIndexDataSlot.h"

DBIndexDataSlot::DBIndexDataSlot(BufType cache, int _dataLen):
    DBSlot(cache)
{
	this -> dataLen = _dataLen;
	fatherPageId = (*this)[FATHER_PAGE_ID_OFFSET];
	isLeaf = (*this)[IS_LEAF_OFFSET];
	dataCnt = (*this)[DATA_CNT_OFFSET];
};

void DBIndexDataSlot::writeData(int idx, char* data, int len){
	 char* dataIterator = (char*)(cache) + (idx) * (dataLen + sizeof(unsigned int)) + sizeof(unsigned int) + size();
	 for(int i = 0; i < len; i++){
	 	dataIterator[i] = data[i];
	 }
}

void DBIndexDataSlot::writePointer(int idx, unsigned int pagenum){
	writeUnsignedInt((BufType)((char*)cache + size() + idx * (dataLen + sizeof(unsigned int))), pagenum);
}

int DBIndexDataSlot::size(){
	return sizeof(unsigned int) * 2 + sizeof(unsigned int);
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

int DBIndexDataSlot::getisLeaf(){
	int re;
	readInt(isLeaf, &re);
	return re;
}

void DBIndexDataSlot::setisLeaf(bool _isLeaf){
	writeInt(isLeaf, (unsigned int)_isLeaf);
}

int DBIndexDataSlot::getDataCnt(){
	int re;
	readInt(dataCnt, &re);
	return re;
}

void DBIndexDataSlot::print(){
	std::cout << "Father Page ID: " << getFatherPageID() << endl;
	std::cout << "Data Count:" << getDataCnt() << endl;
	if (getisLeaf() == 1)
		std::cout << "Leaf Page: " << "true" << endl;
	else
		std::cout << "Leaf Page: " << "false" << endl;
	for(int i = 0; i < getDataCnt(); i++){
		std::cout << "Linker " << i << ", " << "point to page " << getPointerByIdx(i);
		std::cout << " and key is: ";
		for(int j = 0; j < dataLen; j++){
			printf("%02x", ((char*)cache + size() + sizeof(unsigned int) * (i + 1) + dataLen * i + j)[0]);
		}
		printf("\n");
	}
}

int DBIndexDataSlot::getDataSize(){
	return getDataCnt() * (dataLen + sizeof(unsigned int));
}

char* DBIndexDataSlot::getDataByIdx(int idx){
	char* re;
	re = (char*)cache + size() + (idx) * (dataLen + sizeof(unsigned int)) + sizeof(unsigned int);
	return re;
}

unsigned int DBIndexDataSlot::getPointerByIdx(int idx){
	unsigned int re;
	readUnsignedInt((*this)[size() + (idx) * (dataLen + sizeof(unsigned int))], &re);
	return (unsigned int)re;
}

int DBIndexDataSlot::getMaxSize(){
	return (PAGE_SIZE - this -> size()) / (dataLen + sizeof(unsigned int));
}

void DBIndexDataSlot::appendData(BufType data, int size){
	int totsize = getDataSize();
	char* byteCache = (char*) cache;
	char* byteData = (char*) data;
	for(int i = 0; i < size + sizeof(unsigned int); i++){
		byteCache[this -> size() + getDataSize() + i] = byteData[i];
	}
	writeInt(dataCnt, getDataCnt() + 1);
}