#ifndef DBDATAFILEDESCRIPTIONPAGE_H_INCLUDED
#define DBDATAFILEDESCRIPTIONPAGE_H_INCLUDED

#include"DBPage.h"
#include"DBDataFileDescriptionSlot.h"

class DBDataFileDescriptionPage: public DBPage
{
protected:
    DBDataFileDescriptionSlot* dfds;
    DBRecordInfo* ri;

public:
    DBDataFileDescriptionPage(BufType cache, int index, int pageID, int mode, DBRecordInfo* ri);

    void incrementPageNumber(int type);

    int getPageNumber();

    int getRecordLength();

    int getFirstDataPage();

    int getFirstUsagePage();

    int addField(std::string name, int type, bool nullable);

    int setPrimaryKey(std::string name);

    void print();
};

#endif // DBDATAFILEDESCRIPTIONPAGE_H_INCLUDED
