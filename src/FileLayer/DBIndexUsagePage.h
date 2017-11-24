#ifndef DBINDEXUSAGEPAGE_H_INCLUDED
#define DBINDEXUSAGEPAGE_H_INCLUDED

#include "DBIndexUsageSlot.h"
#include "DBPage.h"
#include "../DBInclude.h"

class DBIndexUsagePage: public DBPage
{
protected:
    DBIndexUsageSlot* ius;

public:
    DBIndexUsagePage(BufType cache, int index, int pageID, int mode);

    bool isAvailable(int pageID);

    void setAvailable(int pageID, bool available);

    bool withinRange(int pageID);

    void print();

    int findFirstAvailable();

    int visibleSize();
};

#endif