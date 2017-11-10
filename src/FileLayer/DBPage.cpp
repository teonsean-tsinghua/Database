#include"DBPage.h"

DBPage::DBPage(BufType cache, int index, int pageID, int type, int mode):
    cache(cache), index(index), pageID(pageID)
{
    boundary = (char*)((*this)[PAGE_SIZE]);
    pis = new DBPageInfoSlot((*this)[PAGE_INFO_SLOT_OFFSET]);
}

BufType DBPage::operator[](const int offset) const
{
    return (BufType)((char*)cache + offset);
}

int DBPage::getNextSameType()
{
    return pis->getNextSamePage();
}

void DBPage::setNextSameType(int pid)
{
    pis->setNextSamePage(pid);
}

int DBPage::getIndex()
{
    return index;
}

void DBPage::print()
{
    DBPrint("Page ID: ");
    DBPrintLine(pageID);
    pis->print();
}

int DBPage::getPageType()
{
    return pis->getPageType();
}

int DBPage::getPageID()
{
    return pageID;
}
