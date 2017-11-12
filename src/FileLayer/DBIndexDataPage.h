#ifndef DBINDEXDATAPAGE_H_INCLUDED
#define DBINDEXDATAPAGE_H_INCLUDED
#include "DBIndexDataSlot.h"

class DBIndexDataPage{
private:
	const int GREATER = 0;
	const int LESS = GREATER + 1;
	const int EQUAL = LESS + 1;
	DBIndexDataSlot* ids;
	int dataLen;

	int comparator(char* a, char* b, int len);
public:
	DBIndexDataPage(BufType cache, int _dataLen);

	int insert(char* data, int len, int pointer, int idx);

	int search(char* data);
	
	int deleteByIdx(int idx);

	int getPointerByIdx(int idx);
	
	int getMaxSize();

	const static int SUCCEED = 0;
	const static int OVER_FLOW = SUCCEED + 1;
	const static int UNDEF_FLOW = OVER_FLOW + 1; 
};

#endif