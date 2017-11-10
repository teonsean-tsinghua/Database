#include"DBDataPage.h"

DBDataPage::DBDataPage(BufType cache, int index, int recordLength, int mode, DBRecordInfo* ri):
    DBPage(cache, index, DBType::DATA_PAGE, mode), recordLength(recordLength), ri(ri)
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
        for(int i = 0; i < records.size(); i++)
        {
            std::map<std::string, void*> data;
            records[i]->read(data);
            for(std::map<std::string, void*>::iterator iter = data.begin(); iter != data.end(); iter++)
            {
                int idx = ri->indexes[iter->first];
                if(iter->second == NULL)
                {
                    DBLogLine(iter->first + ": NULL");
                }
                else
                {
                    switch(ri->types[idx])
                    {
                    case DBType::INT:
                        DBLog(iter->first + ": ");
                        DBLogLine(*(int*)(iter->second));
                        break;
                    }
                }
            }
        }
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
    if(cur + recordLength < PAGE_SIZE)
    {
        return SUCCEED;
    }
    return DATA_PAGE_FULL;
}
