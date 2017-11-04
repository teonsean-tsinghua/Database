#ifndef DBRECORD_H_INCLUDED
#define DBRECORD_H_INCLUDED
#include "DBRecordUsage.h"
#include "DBRecordInfo.h"
class DBRecord{
private:
	int recordlen, totalsize;
	unsigned char* data;
public:
	DBRecord();
	void setData(unsigned char* _data, int recordlen, int totalsize);
	void insertData(unsigned char* _data);
	void deleteData(int offset);
	unsigned char* getInfoByOffset(int offset, int _infolen);
	unsigned char* getData();
	bool clearDataByRid(unsigned long long Rid);
	std::vector<unsigned long long>* searchData(const unsigned char* searchData, int offset, int _infolen);
	~DBRecord();
};
#endif