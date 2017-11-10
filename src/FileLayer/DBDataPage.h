#ifndef DBDATAPAGE_H_INCLUDED
#define DBDATAPAGE_H_INCLUDED

#include"DBPage.h"
#include"DBRecordSlot.h"

class DBDataPage: public DBPage
{
protected:
    int recordLength;

public:
    DBDataPage(BufType cache, int index, int recordLength, int mode);
};

#endif // DBDATAPAGE_H_INCLUDED
