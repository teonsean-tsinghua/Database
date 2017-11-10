#ifndef DBUSAGEPAGE_H_INCLUDED
#define DBUSAGEPAGE_H_INCLUDED

#include"DBPage.h"
#include"DBUsageSlot.h"
#include"../DBInclude.h"

class DBUsagePage: public DBPage
{
protected:
    DBUsageSlot* us;

public:
    DBUsagePage(BufType cache, int index, int pageID, int mode);

    bool isAvailable(int pageID);

    void setAvailable(int pageID, bool available);
};

#endif // DBUSAGEPAGE_H_INCLUDED
