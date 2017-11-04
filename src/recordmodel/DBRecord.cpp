#include "DBRecord.h"
#include <cstdio>

DBRecord::DBRecord(){
	data = NULL;
}

void DBRecord::setData(unsigned char* _data, int _recordlen, int _totalsize){
	if (data != NULL){
		delete[] data;
	}
	data = _data;
	recordlen = _recordlen;
	totalsize = _totalsize;
}

void DBRecord::insertData(unsigned char* _data){
	for(int i = 0; i < recordlen; i++){
		data[totalsize + i] = _data[i];
	}
	totalsize = totalsize + recordlen;
}

void DBRecord::deleteData(int offset){
	for(int i = 0; i < recordlen; i++){
		data[offset + recordlen - i - 1] = data[totalsize - i - 1];
	}
	for(int i = totalsize - recordlen; i < totalsize; i++)
		data[i] = 0;
	totalsize = totalsize - recordlen;
}

unsigned char* DBRecord::getInfoByOffset(int offset, int _infolen){
	_infolen = _infolen + 2;
	unsigned char* ret = new unsigned char[8192];
	int recordnum = totalsize / recordlen;
	for(int i = 0; i < recordnum; i++)
		for(int j = 0; j < _infolen; j++)
			ret[i * _infolen + j] = data[i * recordlen + offset + j];
	return ret;
}

std::vector<unsigned long long>* DBRecord::searchData(const unsigned char* searchData, int offset, int _infolen){
	std::vector<unsigned long long>* resdata = new std::vector<unsigned long long>();
	int recordnum = totalsize / recordlen;
	for(int i = 0; i < recordnum; i++){
		int len = data[offset + i * recordlen] * 256 + data[offset + i * recordlen + 1];
		if (len != _infolen)
			continue;
		bool flag = true;
		for(int j = 0; j < _infolen; j++)
			if (searchData[j] != data[offset + i * recordlen + 2 + j]){
				flag = false;
				break;
			}
		if (flag == true){
			unsigned long long tmp = 0;
			for(int j = 0; j < 8; j++){
				tmp = tmp << 8;
				tmp = tmp + data[i * recordlen + j + 2];
			}
			resdata -> push_back(tmp);
		}
	}
	return resdata;
}

bool DBRecord::clearDataByRid(unsigned long long RID){
	int recordnum = totalsize / recordlen;
	for(int i = 0; i < recordnum; i++){
		unsigned long long tmp = 0;
		for(int j = 0; j < 8; j++){
			tmp = tmp << 8;
			tmp = tmp + data[i * recordlen + j + 2];
		}
		if(tmp == RID){
			deleteData(i * recordlen);
			return true;
		}
	}
	return false;
}

unsigned char* DBRecord::getData(){
	return data;
}

DBRecord::~DBRecord(){
	delete[] data;
}