#include"DBUsagePage.h"

DBUsagePage::DBUsagePage(BufType cache, int index, int pid, int mode):
    DBPage(cache, index, pid, DBType::USAGE_PAGE, mode)
{
    us = new DBUsageSlot((*this)[pis->size()]);
    if(mode == MODE_CREATE)
    {
        pis->setFirstAvailableByte(PAGE_SIZE);
        pis->setLengthFixed(true);
        pis->setNextSamePage(-1);
        pis->setPageType(DBType::USAGE_PAGE);
        memset((char*)(*this)[pis->size()], 0, PAGE_SIZE - pis->size());
    }
}

int DBUsagePage::findFirstAvailable()
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

bool DBUsagePage::isAvailable(int pid)
{
    bool re = false;
    readCharToBool((*us)[pid - pageID - 1], &re);
    return re;
}

bool DBUsagePage::withinRange(int pid)
{
    if(pid <= this->pageID || pid > this->pageID + PAGE_SIZE - pis->size())
    {
        return false;
    }
    return true;
}

void DBUsagePage::setAvailable(int pid, bool available)
{
    writeBoolToChar((*us)[pid - pageID - 1], available);
}

DBUsagePage::DBUsageSlot::DBUsageSlot(BufType cache):
    DBSlot(cache)
{

}
