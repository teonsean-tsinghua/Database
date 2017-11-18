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

    void writePointer(int idx, unsigned int pagenum);

    int size();

    int getFatherPageID();

    void setFatherPageID(int _fatherPageId);

    int getisLeaf();

    void setisLeaf(bool _isLeaf);

    int getDataCnt();

    int getDataSize();

    void appendData(BufType data, int size);

    void setDataCnt(int _dataCnt);

    int getMaxSize();

    char* getDataByIdx(int idx);

    unsigned int getPointerByIdx(int idx);

    void print();

    const static int FATHER_PAGE_ID_OFFSET = 0;
    const static int IS_LEAF_OFFSET = FATHER_PAGE_ID_OFFSET + sizeof(unsigned int);
    const static int DATA_CNT_OFFSET = IS_LEAF_OFFSET + sizeof(unsigned int);
};

#endif