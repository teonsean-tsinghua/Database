#include"DBDataFileDescriptionPage.h"

DBDataFileDescriptionPage::DBDataFileDescriptionPage(BufType cache, int index, int pageID, int mode):
    DBPage(cache, index, pageID, DBType::DATA_FILE_DESCRIPTION_PAGE, mode)
{
    dfds = new DBDataFileDescriptionSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()], mode);
    ri = DBRecordInfo::getInstance();
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
    case DBType::USAGE_PAGE:
        if(dfds->getFirstUsagePage() <= 0)
        {
            dfds->setFirstUsagePage(dfds->getPageNumber() - 1);
        }
        break;
    }
}

int DBDataFileDescriptionPage::getFirstDataPage()
{
    return dfds->getFirstDataPage();
}

int DBDataFileDescriptionPage::getFirstUsagePage()
{
    return dfds->getFirstUsagePage();
}

int DBDataFileDescriptionPage::getPageNumber()
{
    return dfds->getPageNumber();
}

int DBDataFileDescriptionPage::setPrimaryKey(std::string name)
{
    int idx = ri->index(name);
    if(idx < 0)
    {
        return FIELD_NOT_EXIST;
    }
    if(idx == dfds->getPrimaryKeyIndex())
    {
        return FIELD_IS_ALREADY_PRIMARY_KEY;
    }
    dfds->setPrimaryKeyIndex(idx);
    //TODO: rebuild the file?
    return SUCCEED;
}

int DBDataFileDescriptionPage::maxRecordInfoLength()
{
    return PAGE_SIZE - (DBPageInfoSlot::size() + DBDataFileDescriptionSlot::minSize());
}

void DBDataFileDescriptionPage::writeFields()
{
    dfds->write();
    pis->setFirstAvailableByte(pis->size() + dfds->size());
}

void DBDataFileDescriptionPage::print()
{
    DBPage::print();
    dfds->print();
}
