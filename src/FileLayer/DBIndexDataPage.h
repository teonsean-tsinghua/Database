#ifndef DBINDEXDATAPAGE_H_INCLUDED
#define DBINDEXDATAPAGE_H_INCLUDED
#include "DBIndexDataSlot.h"
#include "DBPage.h"

class DBIndexDataPage: public DBPage{
private:
	const static int GREATER = 0;
	const static int LESS = GREATER + 1;
	const static int EQUAL = LESS + 1;
	DBIndexDataSlot* ids;
	int dataLen;

	int comparator(char* a, char* b, int len);
public:
	DBIndexDataPage(BufType cache, int index, int pageID, int mode, int _dataLen);

	int insert(char* data, int len, int pointer, int idx, int offset);

	int search(char* data, int len);

	int searchIdx(char* data, int len);
	
	int deleteByIdx(int idx);

	void forceDelete(int idx);
	
	int getMaxSize();

	int getFloatSize();

	int getPointerByIdx(int idx);

	void setIsLeaf(bool _isLeaf);

	void setFatherPageID(int _fatherPageID);

	void setDataCnt(int _dataCnt);

	void updateIdx(char* data, int idx);

	bool getIsLeaf();

	int getFather();

	int getDataCnt();

	int getDataSize();

	int getDataOffsetByIdx(int idx);

	BufType getDataByIdx(int idx);

	BufType getDataHead();

	void appendData(char* data1, int size);

	BufType getCache();

	void setCache(BufType _cache);

	void print();

	const static int SUCCEED = 0;
	const static int OVER_FLOW = SUCCEED + 1;
	const static int UNDER_FLOW = OVER_FLOW + 1; 
};

#endif