#include"DBUsagePage.h"

DBUsagePage::DBUsagePage(BufType cache, int index, int pid, int mode):
    DBPage(cache, index, pid, DBType::USAGE_PAGE, mode)
{
    if(mode == MODE_CREATE)
    {
        pis->setFirstAvailableByte(PAGE_SIZE);
        pis->setLengthFixed(true);
        pis->setNextSamePage(-1);
        pis->setPageType(DBType::USAGE_PAGE);
        memset((char*)(*this)[pis->size()], 0, PAGE_SIZE - pis->size());
    }
    us = new DBUsageSlot((*this)[pis->size()]);
}

int DBUsagePage::findFirstAvailable()
{
    for(int i = pis->size(); i < PAGE_SIZE; i++)
    {
        if(us->isAvailable(i - pis->size()))
        {
            return pageID + i - pis->size() + 1;
        }
    }
    return -1;
}

bool DBUsagePage::isAvailable(int pid)
{
    return us->isAvailable(pid - this->pageID - 1);
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
    us->setAvailable(pid - this->pageID - 1, available);
}
