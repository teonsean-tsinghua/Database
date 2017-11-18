#include "DBIndexDataPage.h"

DBIndexDataPage::DBIndexDataPage(BufType cache, int index, int pageID, int mode, int _dataLen):
	DBPage(cache, index, pageID, DBType::INDEX_DATA_PAGE, mode){
	ids = new DBIndexDataSlot(cache, _dataLen);
	dataLen = _dataLen;
	if(mode == MODE_CREATE)
    {
        ids -> setisLeaf(false);
        ids -> setFatherPageID(-1);
        ids -> setDataCnt(0);
        memset((char*)(*this)[ids -> size()], 0, PAGE_SIZE - ids -> size());
    }
}

int DBIndexDataPage::insert(char* data, int len, int pointer, int idx){
	int currentTotal = ids -> getDataCnt();
	if(currentTotal == ids -> getMaxSize())
		return OVER_FLOW;
	for(int i = currentTotal; i >= idx; i--){
		ids -> writeData(i + 1, ids -> getDatabyIdx(i), dataLen);
		ids -> writePointer(i + 1, ids -> getPointerbyIdx(i));
	}
	ids -> writeData(idx, data, len);
	ids -> writePointer(idx, pointer);
	ids -> setDataCnt(ids -> getDataCnt() + 1);
	return SUCCEED;
}

int DBIndexDataPage::deletebyIdx(int idx){
	int currentTotal = ids -> getDataCnt();
	if (currentTotal == (ids -> getMaxSize()) >> 1 && (ids -> getFatherPageID() != -1))
		return UNDER_FLOW;
	if (ids -> getFatherPageID() == -1 && currentTotal == 1)
		return UNDER_FLOW;
	for(int i = idx; i < currentTotal; i++){
		ids -> writeData(i, ids -> getDatabyIdx(i + 1), dataLen);
		ids -> writePointer(i, ids -> getPointerbyIdx(i + 1));
	}
	ids -> setDataCnt(ids -> getDataCnt() - 1);
	return SUCCEED;
}

void DBIndexDataPage::updateIdx(char* data, int idx){
	ids -> writeData(idx, data, dataLen);
}

void DBIndexDataPage::forceDelete(int idx){
	int currentTotal = ids -> getDataCnt();
	for(int i = idx; i < currentTotal; i++){
		ids -> writeData(i, ids -> getDatabyIdx(i + 1), dataLen);
		ids -> writePointer(i, ids -> getPointerbyIdx(i + 1));
	}
	ids -> setDataCnt(ids -> getDataCnt() - 1);
}

int DBIndexDataPage::comparator(char* a, char* b, int len){
	for(int i = 0; i < len; i++){
		if (a[i] < b[i]) return LESS;
		if (a[i] > b[i]) return GREATER;
	}
	return EQUAL;
}

int DBIndexDataPage::getDataOffsetbyIdx(int idx){
	return ids -> size() + (dataLen + sizeof(unsigned int)) * idx + sizeof(unsigned int);
}

int DBIndexDataPage::getMaxSize(){
	return ids -> getMaxSize();
}

int DBIndexDataPage::getFloatSize(){
	return (ids -> getDataCnt()) - ((ids -> getMaxSize()) >> 1);
}

int DBIndexDataPage::search(char* data, int len){
	for(int i = 0; i < ids -> getDataCnt(); i++){
		if (comparator(data, ids -> getDatabyIdx(i), dataLen) != GREATER)
			return ids -> getPointerbyIdx(i);
	}
	return ids -> getPointerbyIdx(ids -> getDataCnt() - 1);
}

BufType DBIndexDataPage::getCache(){
	return cache;
}

BufType DBIndexDataPage::getDataHead(){
	return cache + ids -> size() / sizeof(int);
}

void DBIndexDataPage::appendData(char* data, int size){
	ids -> appendData(data, size);
}

void DBIndexDataPage::setCache(BufType _cache){
	for(int i = 0; i < (PAGE_SIZE >> 2); i++)
		cache[i] = _cache[i];
}

bool DBIndexDataPage::getisLeaf(){
	return ids -> getisLeaf();
}

void DBIndexDataPage::setisLeaf(bool _isLeaf){
	ids -> setisLeaf(_isLeaf);
}

void DBIndexDataPage::setFatherPageID(int _fatherPageID){
	ids -> setFatherPageID(_fatherPageID);
}

void DBIndexDataPage::setDataCnt(int _dataCnt){
	ids -> setDataCnt(_dataCnt);
}

int DBIndexDataPage::getPointerbyIdx(int idx){
	return ids -> getPointerbyIdx(idx);
}

int DBIndexDataPage::getDataCnt(){
	return ids -> getDataCnt();
}

int DBIndexDataPage::getDataSize(){
	return ids -> getDataSize();
}

int DBIndexDataPage::getFather(){
	return ids -> getFatherPageID();
}

void DBIndexDataPage::print(){
	ids -> print();
}