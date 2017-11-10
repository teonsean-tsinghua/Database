#include"DBDataFileDescriptionPage.h"

DBDataFileDescriptionPage::DBDataFileDescriptionPage(BufType cache, int index, int mode):
    DBPage(cache, index, DBType::DATA_FILE_DESCRIPTION_PAGE, mode)
{
    dfds = new DBDataFileDescriptionSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()], mode);
    if(mode == MODE_CREATE)
    {
        pis->setPageType(DBType::DATA_FILE_DESCRIPTION_PAGE);
        pis->setNextSamePage(-1);
        pis->setLengthFixed(0);
        pis->setFirstAvailableByte(pis->size() + dfds->size());
        dfds->setFirstDataPage(-1);
        dfds->setFirstUsageSlot(-1);
        dfds->setLastUsageSlot(-1);
        dfds->setRecordInfoLength(0);
    }
}

int DBDataFileDescriptionPage::setPrimaryKey(std::string name)
{
    int idx = dfds->getIndexOfField(name);
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
