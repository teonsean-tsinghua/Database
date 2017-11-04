#ifndef DBRECORDUSAGE_H_INCLUDED
#define DBRECORDUSAGE_H_INCLUDED

class DBRecordUsage{
private:
	unsigned char* data;
public:
	DBRecordUsage(unsigned char* _data);
	~DBRecordUsage();
	void setdata(unsigned char* _data);
	int allocateNewRecord(int size);
};

#endif