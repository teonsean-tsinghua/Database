#ifndef DBDATAFILEDESCRIPTIONPAGE_H_INCLUDED
#define DBDATAFILEDESCRIPTIONPAGE_H_INCLUDED

#include"DBPage.h"
#include"DBDataFileDescriptionSlot.h"

class DBDataFileDescriptionPage: public DBPage
{
protected:
    DBDataFileDescriptionSlot* dfds;

public:
    DBDataFileDescriptionPage(BufType cache, int index, int mode);

    void incrementPageNumber();

    int getPageNumber();

    int getRecordLength();

    int addField(std::string name, int type, bool nullable);

    int setPrimaryKey(std::string name);

    int processRawData(std::map<std::string, void*>& raw,
                       std::map<int, void*>& processed,
                       std::map<std::string, int>& errors);

    void print();

    const static int MISSING_FIELD = 0;
    const static int EXTRA_FIELD = 1;
    const static int EDIT__ID = 2;
};

#endif // DBDATAFILEDESCRIPTIONPAGE_H_INCLUDED
