#include"DBPage.h"

DBPage::DBPage(BufType cache, int index, int type, bool parse):
    cache(cache), index(index)
{
    boundary = (char*)((*this)[PAGE_SIZE]);
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
    if(!parse)
    {
        pis->setPageType(DBType::DATA_FILE_DESCRIPTION_PAGE);
        pis->setNextSamePage(-1);
        pis->setLengthFixed(1);
        pis->setFirstAvailableByte(pis->size() + dfds->size());
        dfds->setFirstDataPage(-1);
        dfds->setFirstUsageSlot(-1);
        dfds->setLastUsageSlot(-1);
        dfds->setRecordInfoLength(0);
    }
}

int DBDataFileDescriptionPage::addField(std::string name, int type)
{
    int re = dfds->addField(name, type, boundary);
    if(re == SUCCEED)
    {
        dfds->write();
        pis->setFirstAvailableByte(pis->size() + dfds->size());
    }
    return re;
}

void DBDataFileDescriptionPage::print()
{
    pis->print();
    dfds->print();
}
