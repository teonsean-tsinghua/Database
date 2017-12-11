#include"Page.h"

Page::Page(BufType cache, int index, int pageID, int type):
    cache(cache), index(index), pageID(pageID)
{
    boundary = (char*)((*this)[PAGE_SIZE]);
    pis = new PageInfoSlot((*this)[PAGE_INFO_SLOT_OFFSET]);
}

BufType Page::operator[](const int offset) const
{
    return (BufType)((char*)cache + offset);
}

int Page::getNextSameType()
{
    return pis->getNextSamePage();
}

void Page::setNextSameType(int pid)
{
    pis->setNextSamePage(pid);
}

int Page::getIndex()
{
    return index;
}

void Page::print()
{
    Print::print("Page ID: ").printLine(pageID);
    pis->print();
}

int Page::getPageType()
{
    return pis->getPageType();
}

int Page::getPageID()
{
    return pageID;
}
