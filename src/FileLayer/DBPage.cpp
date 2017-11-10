#include"DBPage.h"

DBPage::DBPage(BufType cache, int index, int type, int mode):
    cache(cache), index(index)
{
    boundary = (char*)((*this)[PAGE_SIZE]);
    pis = new DBPageInfoSlot((*this)[PAGE_INFO_SLOT_OFFSET], type, mode);
}

BufType DBPage::operator[](const int offset) const
{
    return (BufType)((char*)cache + offset);
}

int DBPage::getIndex()
{
    return index;
}

void DBPage::print()
{
    pis->print();
}

