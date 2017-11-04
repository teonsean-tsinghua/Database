#include "DBRecordUsage.h"
#include "../utils/pagedef.h"
#include "../utils/DBLog.h"

DBRecordUsage::DBRecordUsage(unsigned char* _data):data(_data){
}

DBRecordUsage::DBRecordUsage(){
	data = NULL;
}

DBRecordUsage::~DBRecordUsage(){
	delete[] data;
}

void DBRecordUsage::setData(unsigned char* _data){
	if (data != NULL){
		delete[] data;
	}
	data = _data;
}

int DBRecordUsage::allocateNewRecord(int size){
	int dataval = 0;
	for(int i = 0; i < (PAGE_SIZE >> 1); i++){
		dataval = data[i * 2] * 256 + data[i * 2 + 1];
		if (dataval > size){
			dataval = dataval - size;
			char* msg = new char[64];
			data[i * 2] = dataval >> 8;
			data[i * 2 + 1] = dataval % 256;
			sprintf(msg, "Allocate at page %d, free space = %d", i, dataval);
			log(std::string(msg));
			return i;
		}
	}
	return -1;
}

void DBRecordUsage::releaseSize(int releaseIdx, int size){
	int dataval = data[releaseIdx * 2] * 256 + data[releaseIdx * 2 + 1];
	dataval = dataval + size;
	data[releaseIdx * 2] = dataval >> 8;
	data[releaseIdx * 2 + 1] = dataval % 256;
}

int DBRecordUsage::getUsageByPage(int page){
	int dataval = (int) data[page * 2] * 256 + (int)data[page * 2 + 1];
	return dataval;
}

unsigned char* DBRecordUsage::getData(){
	return data;
}