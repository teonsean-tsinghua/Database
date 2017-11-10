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

int DBDataFileDescriptionPage::processRawData(std::map<std::string, void*>& raw,
                                              std::map<int, void*>& processed,
                                              std::map<std::string, int>& errors)
{
    processed.clear();
    errors.clear();
    std::map<std::string, void*>::iterator iter;
    std::vector<bool> included;
    included.assign(dfds->getFieldCount(), false);
    for(iter = raw.begin(); iter != raw.end(); iter++)
    {
        int idx = dfds->getIndexOfField(iter->first);
        if(idx < 0)
        {
            errors[iter->first] = EXTRA_FIELD;
            continue;
        }
        if(idx == 0)
        {
            errors[iter->first] = EDIT__ID;
            continue;
        }
        included[idx] = true;
        processed[dfds->getOffsetOfField(idx)] = iter->second;
    }
    for(int i = 1; i < included.size(); i++)
    {
        if(!included[i] && !dfds->getNullableOfField(i))
        {
            errors[dfds->getNameOfField(i)] = MISSING_FIELD;
        }
    }
    if(errors.empty())
    {
        return SUCCEED;
    }
    return ERROR;
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
