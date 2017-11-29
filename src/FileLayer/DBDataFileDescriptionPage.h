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
    DBDataFileDescriptionPage(BufType cache, int index, int pageID, int mode);

    void incrementPageNumber(int type);

    int getPageNumber();

    int getFirstDataPage();

    int getFirstUsagePage();

    void writeFields();

    int setPrimaryKey(std::string name);

    void print();

    static int maxRecordInfoLength();
};

#endif // DBDATAFILEDESCRIPTIONPAGE_H_INCLUDED
