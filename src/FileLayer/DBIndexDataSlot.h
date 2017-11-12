#ifndef DBINDEXDATASLOT_H_INCLUDED
#define DBINDEXDATASLOT_H_INCLUDED
#include "DBSlot.h"

class DBIndexDataSlot: public DBSlot{
private:
	int dataLen;
	BufType fatherPageId;
	BufType isLeaf;
	BufType dataCnt;
public:
    DBIndexDataSlot(BufType cache, int _dataLen);

    void writeData(int idx, char* data, int len);

    void writePointer(int idx, BufType pagenum);

    int size();

    void print();

    int getFatherPageId();

    int getIsLeaf();

    int getDataCnt();

    int getMaxSize();

    char* getDataByIdx(int idx);

    BufType getPointerByIdx(int idx);

    void writeFatherPageId(int _fatherPageId);

    void writeIsLeaf(bool isLeaf);

    void writeDataCnt(int _dataCnt);

    const static int FATHER_PAGE_ID_OFFSET = 0;
    const static int IS_LEAF_OFFSET = FATHER_PAGE_ID_OFFSET + 1;
    const static int DATA_CNT_OFFSET = IS_LEAF_OFFSET + 1;
};

#endif