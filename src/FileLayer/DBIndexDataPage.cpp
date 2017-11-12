#include "DBIndexDataPage.h"

DBIndexDataPage::DBIndexDataPage(BufType cache, int index, int pageID, int mode, int _dataLen):
	ids(cache, _dataLen), DBPage(cache, index, pageID, DBType::INDEX_DATA_PAGE, mode){
	dataLen = _dataLen;
}

int DBIndexDataPage::insert(char* data, int len, int pointer, int idx){
	int currentTotal = ids -> getDataCnt();
	if(currentTotal == ids -> getMaxSize())
		return OVER_FLOW;
	for(int i = currentTotal; i > idx; i--){
		ids -> writeData(i + 1, ids -> getDataByIdx(i), _dataLen);
		ids -> writePointer(i + 1, ids -> getPointerByIdx(i));
	}
	ids -> writeData(idx, data, len);
	idx -> writePointer(idx, &pointer);
	return SUCCEED;
}

void DBIndexDataPage::deleteByIdx(int idx){
	int currentTotal = ids -> getDataCnt();
	if (currentTotal == (ids -> getMaxSize()) >> 1)
		return UNDER_FLOW;
	for(int i = idx; i < currentTotal; i++){
		ids -> writeData(i, ids -> getDataByIdx(i + 1), _dataLen);
		ids -> writePointer(i, ids -> getPointerByIdx(i + 1));
	}
	return SUCCEED;
}

int DBIndexDataPage::comparator(char* a, char* b, int len){
	for(int i = 0; i < len; i++){
		if (a[i] < b[i]) return LESS;
		if (a[i] > b[i]) return GREATER;
	}
	return EQUAL;
}

int DBIndexDataPage::getMaxSize(){
	return ids -> getMaxSize();
}

int DBIndexDataPage::getPointerByIdx(int idx){
	return ids -> getPointerByIdx(idx);
}

int DBIndexDataPage::search(char* data){
	for(int i = 0; i < ids -> getDataCnt(); i++)
		if (comparator(data, ids -> getDataByIdx(i), dataLen) == GREATER)
			return ids -> getPointerByIdx(i);
	return ids -> getPointerByIdx(ids -> getDataCnt());
}

BufType DBIndexDataPage::getCache(){
	return cache;
}

void DBIndexDataPage::getCache(BufType _cache){
	for(int i = 0; i < (PAGE_SIZE >> 2); i++)
		cache[i] = _cache[i];
}