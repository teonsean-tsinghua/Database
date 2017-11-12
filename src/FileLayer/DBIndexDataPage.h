#ifndef DBINDEXDATAPAGE_H_INCLUDED
#define DBINDEXDATAPAGE_H_INCLUDED
#include "DBIndexDataSlot.h"

class DBIndexDataPage{
private:
	DBIndexDataSlot* ids;
	int dataLen;
public:
	DBIndexDataPage(BufType cache, int _dataLen);
	int insert();
	int delete();

	const static int SUCCEED = 0;
	const static int OVER_FLOW = SUCCEED + 1;
	const static int UNDEF_FLOW = OVER_FLOW + 1; 
};

#endif