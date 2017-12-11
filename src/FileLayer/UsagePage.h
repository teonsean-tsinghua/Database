#ifndef USAGEPAGE_H_INCLUDED
#define USAGEPAGE_H_INCLUDED

#include"Page.h"
#include"Slot.h"
#include"../Include.h"

class UsagePage: public Page
{
protected:
    class UsageSlot:public Slot
    {
    public:
        UsageSlot(BufType cache);
    };

    UsageSlot* us;

public:
    UsagePage(BufType cache, int index, int pageID, bool parse);

    bool isAvailable(int pageID);

    void setAvailable(int pageID, bool available);

    bool withinRange(int pageID);

    int findFirstAvailable();
};

#endif // USAGEPAGE_H_INCLUDED
