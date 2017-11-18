#ifndef DBINDEXUSAGEPAGE_H_INCLUDED
#define DBINDEXUSAGEPAGE_H_INCLUDED

#include"../DBInclude.h"

class DBIndexUsagePage: public DBPage
{
protected:
    DBIndexUsageSlot* us;

public:
    DBUsagePage(BufType cache, int index, int pageID, int mode);

    bool isAvailable(int pageID);

    void setAvailable(int pageID, bool available);

    bool withinRange(int pageID);

    int findFirstAvailable();

    int visibleSize();
};

#endif