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

	int insert(char* data, int len, unsigned int* pointer, int idx);

	BufType search(char* data, int len);
	
	int deleteByIdx(int idx);

	void forceDelete(int idx);

	BufType getPointerByIdx(int idx);
	
	int getMaxSize();

	int getFloatSize();

	int getPointerbyIdx(int idx);

	void updateIdx(BufType data, int idx);

	bool getisLeaf();

	int getFather();

	int getDataCnt();

	int getDataSize();

	int getOffsetbyIdx(int idx);

	BufType getDataHead();

	void appendData(BufType data1, int size);

	void refresh();

	int searchIdx(const char* data, int len);

	BufType getCache();

	void setCache(BufType _cache);

	const static int SUCCEED = 0;
	const static int OVER_FLOW = SUCCEED + 1;
	const static int UNDER_FLOW = OVER_FLOW + 1; 
};

#endif