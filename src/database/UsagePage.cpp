#include"UsagePage.h"

UsagePage::UsagePage(BufType cache, int index, int pid, bool parse):
    Page(cache, index, pid, Type::USAGE_PAGE)
{
    us = new UsageSlot((*this)[pis->size()]);
    if(!parse)
    {
        pis->setFirstAvailableByte(PAGE_SIZE);
        pis->setLengthFixed(true);
        pis->setNextSamePage(-1);
        pis->setPageType(Type::USAGE_PAGE);
        memset((char*)(*this)[pis->size()], 0, PAGE_SIZE - pis->size());
    }
}

int UsagePage::findFirstAvailable()
{
    int head = pageID + 1;
    int end = pageID + PAGE_SIZE - pis->size();
    for(int i = head; i <= end; i++)
    {
        if(isAvailable(i))
        {
            return i;
        }
    }
    return -1;
}

bool UsagePage::isAvailable(int pid)
{
    bool re = false;
    readCharToBool((*us)[pid - pageID - 1], &re);
    return re;
}

bool UsagePage::withinRange(int pid)
{
    if(pid <= this->pageID || pid > this->pageID + PAGE_SIZE - pis->size())
    {
        return false;
    }
    return true;
}

void UsagePage::setAvailable(int pid, bool available)
{
    writeBoolToChar((*us)[pid - pageID - 1], available);
}

UsagePage::UsageSlot::UsageSlot(BufType cache):
    Slot(cache)
{

}
