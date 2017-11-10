#include"DBUsagePage.h"

DBUsagePage::DBUsagePage(BufType cache, int index, int pageID, int mode):
    DBPage(cache, index, pageID, DBType::USAGE_PAGE, mode)
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

bool DBUsagePage::isAvailable(int pageID)
{
    if(pageID <= this->pageID || pageID > this->pageID + PAGE_SIZE - pis->size())
    {
        return false;
    }
    return us->isAvailable(pageID - this->pageID - 1);
}

void DBUsagePage::setAvailable(int pageID, bool available)
{
    if(pageID <= this->pageID || pageID > this->pageID + PAGE_SIZE - pis->size())
    {
        return;
    }
    us->setAvailable(pageID - this->pageID - 1, available);
}
