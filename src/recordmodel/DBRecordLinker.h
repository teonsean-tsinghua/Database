// This file is for create a linker for DBRecordLinker contains DBRecordInfo
#ifndef DBRECORD_RECORD_LINKER_INCLUDE
#define DBRECORD_RECORD_LINKER_INCLUDE

#include "DBRecordInfo.h"
#include <vector>

class DBLinkedRecord{
private:
	DBRecordInfo record;
	DBLinkedRecord *previousRecord, *nextRecord;
	bool debugmode;
	int keyvalue;
public:
	DBLinkedRecord(DBRecordInfo _record, bool _isDebugMode);
	void setPreviousRecord(DBLinkedRecord *_previousRecord);
	void setNextRecord(DBLinkedRecord *_nextRecord);
	DBRecordInfo getRecord();
	DBLinkedRecord* getPreviousRecord();
	DBLinkedRecord* getNextRecord();
	int getKeyValue();
};

class DBRecordLinker{
private:
	DBLinkedRecord* linker;
	bool debugmode;
	int linkersize;
	void throwLastRecord();
	void getFirstRecord();
	DBRecordLinker *_previouslinker, *_nextlinker;
public:
	DBRecordLinker(bool _debugmode);
	bool insertRecord(DBLinkedRecord _linkedRecord, int keyvalue);
	bool deleteRecord(int _keyvalue);
	DBLinkedRecord* searchRecord(int _keyvalue);
	DBRecordLinker* getPreviousLinker();
	DBRecordLinker* getNextLinker();
};

#endif