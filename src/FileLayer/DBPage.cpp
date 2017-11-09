#include"DBPage.h"

DBPage::DBPage(BufType cache, int index, int type, bool parse):
    cache(cache), index(index)
{
    boundary = (*this)[PAGE_SIZE];
    pis = new DBPageInfoSlot((*this)[PAGE_INFO_SLOT_OFFSET], type, parse);
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

}

DBDataFileDescriptionPage::DBDataFileDescriptionPage(BufType cache, int index, bool parse):
    DBPage(cache, index, DBType::DATA_FILE_DESCRIPTION_PAGE, parse)
{
    dfds = new DBDataFileDescriptionSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()], parse);
}

void DBDataFileDescriptionPage::addField(std::string name, int type)
{
    dfds->addField(name, type);
    pis->setFirstAvailableByte(pis->size() + dfds->size());
}

void DBDataFileDescriptionPage::write()
{
    pis->write(pis->size() + dfds->size(), true, 0);
    dfds->write(0, 0, 0);
}

void DBDataFileDescriptionPage::print()
{
    pis->print();
    dfds->print();
}
