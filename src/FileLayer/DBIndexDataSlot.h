#ifndef DBINDEXDATASLOT_H_INCLUDED
#define DBINDEXDATASLOT_H_INCLUDED
#include "DBSlot.h"

class DBIndexDataSlot: public DBSlot{
private:
	int dataLen;
	BufType fatherPageId;
	BufType isLeaf;
	BufType dataCnt;
    BufType pageType;
public:
    DBIndexDataSlot(BufType cache, int _dataLen);

    void writeData(int idx, char* data, int len);

    void writePointer(int idx, unsigned int pagenum);

    void writeOffset(int idx, int offset);

    int size();

    int getFatherPageID();

    void setFatherPageID(int _fatherPageId);

    int getIsLeaf();

    void setIsLeaf(bool _isLeaf);

    int getDataCnt();

    int getDataSize();

    void appendData(char* data, int size);

    void setDataCnt(int _dataCnt);

    int getMaxSize();

    char* getDataByIdx(int idx);

    unsigned int getPointerByIdx(int idx);

    int getOffsetByIdx(int idx);

    int getPageType();

    void setPageType(int type);

    void print();

    const static int PAGE_TYPE_OFFSET = 0;
    const static int IS_LEAF_OFFSET = PAGE_TYPE_OFFSET + sizeof(unsigned int);
    const static int DATA_CNT_OFFSET = IS_LEAF_OFFSET + sizeof(unsigned int);
    const static int FATHER_PAGE_ID_OFFSET = DATA_CNT_OFFSET + sizeof(unsigned int);
};

#endif