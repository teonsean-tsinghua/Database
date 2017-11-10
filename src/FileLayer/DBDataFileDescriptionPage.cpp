#include"DBDataFileDescriptionPage.h"

DBDataFileDescriptionPage::DBDataFileDescriptionPage(BufType cache, int index, int mode, DBRecordInfo* ri):
    DBPage(cache, index, DBType::DATA_FILE_DESCRIPTION_PAGE, mode), ri(ri)
{
    dfds = new DBDataFileDescriptionSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()], mode, ri);
    if(mode == MODE_CREATE)
    {
        pis->setPageType(DBType::DATA_FILE_DESCRIPTION_PAGE);
        pis->setNextSamePage(-1);
        pis->setLengthFixed(false);
        pis->setFirstAvailableByte(pis->size() + dfds->size());
        dfds->setFirstDataPage(-1);
        dfds->setFirstUsagePage(-1);
        dfds->setPageNumber(1);
        dfds->setRecordInfoLength(0);
    }
}

void DBDataFileDescriptionPage::incrementPageNumber(int type)
{
    dfds->setPageNumber(dfds->getPageNumber() + 1);
    switch(type)
    {
    case DBType::DATA_PAGE:
        if(dfds->getFirstDataPage() <= 0)
        {
            dfds->setFirstDataPage(dfds->getPageNumber() - 1);
        }
        break;
    }
}

int DBDataFileDescriptionPage::getPageNumber()
{
    return dfds->getPageNumber();
}

int DBDataFileDescriptionPage::getRecordLength()
{
    return dfds->getRecordLength();
}

int DBDataFileDescriptionPage::setPrimaryKey(std::string name)
{
    if(!ri->indexes.count(name))
    {
        return FIELD_NOT_EXIST;
    }
    int idx = ri->indexes[name];
    if(idx == dfds->getPrimaryKeyIndex())
    {
        return FIELD_IS_ALREADY_PRIMARY_KEY;
    }
    dfds->setPrimaryKeyIndex(idx);
    //TODO: rebuild the file?
    return SUCCEED;
}

int DBDataFileDescriptionPage::addField(std::string name, int type, bool nullable)
{
    int re = dfds->addField(name, type, nullable ? 1 : 0, boundary);
    if(re == SUCCEED)
    {
        dfds->write();
        pis->setFirstAvailableByte(pis->size() + dfds->size());
    }
    return re;
}

void DBDataFileDescriptionPage::print()
{
    DBPage::print();
    dfds->print();
}
