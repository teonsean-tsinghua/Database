#ifndef DBRECORDUSAGE_H_INCLUDED
#define DBRECORDUSAGE_H_INCLUDED

class DBRecordUsage{
private:
	unsigned char* data;
public:
	DBRecordUsage(unsigned char* _data);
	DBRecordUsage();
	~DBRecordUsage();
	void setData(unsigned char* _data);
	void releaseSize(int releaseIdx, int size);
	int getUsageByPage(int page);
	int allocateNewRecord(int size);
	unsigned char* getData();
};

#endif