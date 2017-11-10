#include"DBDataPage.h"

DBDataPage::DBDataPage(BufType cache, int index, int pageID, int recordLength, int mode, DBRecordInfo* ri):
    DBPage(cache, index, pageID, DBType::DATA_PAGE, mode), recordLength(recordLength), ri(ri)
{
    if(mode == MODE_CREATE)
    {
        pis->setPageType(DBType::DATA_PAGE);
        pis->setNextSamePage(-1);
        pis->setLengthFixed(true);
        pis->setFirstAvailableByte(pis->size());
        records.clear();
    }
    else if(mode == MODE_PARSE)
    {
        int cur = pis->size(), end = pis->getFirstAvailableByte();
        while(cur < end)
        {
            records.push_back(new DBRecordSlot((*this)[cur], ri));
            cur += recordLength;
        }
    }
}

void DBDataPage::print()
{
    DBPage::print();
    DBLog("This page contains ");
    DBLog(records.size());
    DBLogLine(" records.");
}

void DBDataPage::printAllRecords()
{
    for(int i = 0; i < records.size(); i++)
    {
        records[i]->print();
    }
}

int DBDataPage::insert(std::vector<void*>& data)
{
    int cur = pis->getFirstAvailableByte();
    DBRecordSlot* slot = new DBRecordSlot((*this)[cur], ri);
    int re = slot->write(data);
    if(re != SUCCEED)
    {
        return ERROR;
    }
    cur += recordLength;
    pis->setFirstAvailableByte(cur);
    records.push_back(slot);
    if(cur + recordLength < PAGE_SIZE)
    {
        return SUCCEED;
    }
    return DATA_PAGE_FULL;
}
