#include "DBIndexDataSlot.h"

DBIndexDataSlot::DBIndexDataSlot(BufType cache, int _dataLen):
    DBSlot(cache)
{
	this -> dataLen = _dataLen;
	fatherPageId = (*this)[FATHER_PAGE_ID_OFFSET];
	isLeaf = (*this)[IS_LEAF_OFFSET];
	dataCnt = (*this)[DATA_CNT_OFFSET];
};

void DBIndexDataSlot::refresh(){
	int _fatherPageId = fatherPageId[0];
	int _isLeaf = isLeaf[0];
	int _dataCnt = dataCnt[0];
	cache = new unsigned int[PAGE_SIZE >> 2];
	cache[0] = _fatherPageId;
	cache[1] = _isLeaf;
	cache[2] = _dataCnt;
}

void DBIndexDataSlot::writeData(int idx, char* data, int len){
	 char* dataIterator = ((char*)cache + (idx - 1) * dataLen + 4 * idx + size());
	 for(int i = 0; i < len; i++){
	 	dataIterator[i] = data[i];
	 }
}

void DBIndexDataSlot::writePointer(int idx, BufType pagenum){
	int* intIterator = (int*)((char*)cache + (idx - 1) * (dataLen + 4) + size());
	intIterator[0] = pagenum[0];
}

int DBIndexDataSlot::size(){
	return sizeof(int) * 2 + sizeof(int);
}

void DBIndexDataSlot::print(){
	// TODO: write some debugging info that may be used in the future.
}

int DBIndexDataSlot::getFatherPageID(){
	int re;
	readInt(fatherPageId, &re);
	return re;
}

int DBIndexDataSlot::getisLeaf(){
	int re;
	readInt(isLeaf, &re);
	return re;
}

int DBIndexDataSlot::getDataCnt(){
	int re;
	readInt(dataCnt, &re);
	return re;
}

void DBIndexDataSlot::writeFatherPageID(int _fatherPageId){
	writeInt(fatherPageId, _fatherPageId);
}

void DBIndexDataSlot::writeIsLeaf(bool _isLeaf){
	writeInt(isLeaf, _isLeaf? 1 : 0);
}

void DBIndexDataSlot::writeDataCnt(int _dataCnt){
	writeInt(dataCnt, _dataCnt);
}

char* DBIndexDataSlot::getDataByIdx(int idx){
	char* re;
	re = (char*)cache + size() + (idx - 1) * (dataLen + 4) + 4;
	return re;
}

BufType DBIndexDataSlot::getPointerByIdx(int idx){
	BufType re;
	re = (BufType)((char*)cache + size() + (idx - 1) * (dataLen + 4));
	return re;
}

int DBIndexDataSlot::getMaxSize(){
	return (PAGE_SIZE - size() - 4) / dataLen;
}