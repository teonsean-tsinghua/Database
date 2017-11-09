#include"DBPage.h"

DBPage::DBPage(BufType cache, int index, bool parse):
    cache(cache), index(index)
{
    boundary = (*this)[PAGE_SIZE];
    pis = new DBPageInfoSlot((*this)[PAGE_INFO_SLOT_OFFSET], parse);
}

BufType DBPage::operator[](const int offset) const
{
    return (BufType)((char*)cache + offset);
}

int DBPage::getIndex()
{
    return index;
}

DBDataFileDescriptionPage::DBDataFileDescriptionPage(BufType cache, int index, bool parse):
    DBPage(cache, index, parse)
{
    dfds = new DBDataFileDescriptionSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()], parse);
}

void DBDataFileDescriptionPage::addField(std::string name, int type)
{
    dfds->addField(name, type);
}

void DBDataFileDescriptionPage::write()
{
    pis->write();
    dfds->write(0, 0, 0);
}

void DBDataFileDescriptionPage::printFileDescription()
{
    dfds->printFileDescription();
}
