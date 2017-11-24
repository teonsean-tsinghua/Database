#include "DBIndexUsagePage.h"

DBIndexUsagePage::DBIndexUsagePage(BufType cache, int index, int pid, int mode):
    DBPage(cache, index, pid, DBType::USAGE_PAGE, mode)
{
    if(mode == MODE_CREATE)
    {
        pis->setFirstAvailableByte(PAGE_SIZE);
        pis->setLengthFixed(true);
        pis->setNextSamePage(-1);
        pis->setPageType(DBType::INDEX_USAGE_PAGE);
        memset((char*)(*this)[pis->size()], 1, PAGE_SIZE - pis->size());
    }
    ius = new DBIndexUsageSlot((*this)[pis->size()]);
}

int DBIndexUsagePage::findFirstAvailable()
{   
    for(int i = pis->size(); i < PAGE_SIZE; i++)
    {
        if(ius->isAvailable(i - pis->size()))
        {
            return pageID + i - pis -> size() + 1;
        }
    }
    return -1;
}

bool DBIndexUsagePage::isAvailable(int pid)
{
    return ius->isAvailable(pid - this->pageID - 1);
}

bool DBIndexUsagePage::withinRange(int pid)
{
    if(pid <= this->pageID || pid > this->pageID + PAGE_SIZE - pis->size())
    {
        return false;
    }
    return true;
}

void DBIndexUsagePage::print(){
    for(int i = 0; i < PAGE_SIZE - pis -> size(); i++)
        if (ius -> isAvailable(i))
            std::cout << 1;
        else
            std::cout << 0;
    std::cout << endl;
}

void DBIndexUsagePage::setAvailable(int pid, bool available)
{
    ius->setAvailable(pid - this->pageID - 1, available);
}

int DBIndexUsagePage::visibleSize(){
    return PAGE_SIZE - pis -> size();
}
