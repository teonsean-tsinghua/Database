#include "DBRecordUsage.h"
#include "../utils/pagedef.h"
#include "../utils/DBLog.h"

DBRecordUsage::DBRecordUsage(unsigned char* _data):data(_data){
}

DBRecordUsage::~DBRecordUsage(){
	delete[] data;
}

void DBRecordUsage::setdata(unsigned char* _data){
	delete[] data;
	data = _data;
}

int DBRecordUsage::allocateNewRecord(int size){
	unsigned short* dataval = (unsigned short*) data;
	for(int i = 0; i < (PAGE_SIZE >> 1); i++){
		if (dataval[i] > size){
			dataval[i] = dataval[i] - size;
			char* msg = new char[64];
			sprintf(msg, "Allocate at page %d", i);
			log(std::string(msg));
			return i;
		}
	}
	return -1;
}