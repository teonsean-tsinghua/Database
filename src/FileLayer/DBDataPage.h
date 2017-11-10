#ifndef DBDATAPAGE_H_INCLUDED
#define DBDATAPAGE_H_INCLUDED

#include"../DBInclude.h"
#include"DBPage.h"
#include"DBRecordSlot.h"

class DBDataPage: public DBPage
{
protected:
    int recordLength;
    std::vector<DBRecordSlot*> records;
    DBRecordInfo* ri;

public:
    DBDataPage(BufType cache, int index, int pageID, int recordLength, int mode, DBRecordInfo* ri);

    int insert(std::vector<void*>& data);

};

#endif // DBDATAPAGE_H_INCLUDED
