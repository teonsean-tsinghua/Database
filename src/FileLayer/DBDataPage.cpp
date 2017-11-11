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

int DBDataPage::update(std::map<int, void*>& key_value, std::map<int, void*>& update_value)
{
    for(int i = 0; i < records.size(); i++)
    {
        if(records[i]->equal(key_value) == EQUAL_RECORD)
        {
            records[i]->update(update_value);
        }
    }
    return SUCCEED;
}

int DBDataPage::remove(std::map<int, void*>& data)
{
    std::vector<bool> removed;
    removed.assign(records.size(), false);
    int cnt = 0;
    for(int i = 0; i < records.size(); i++)
    {
        if(records[i]->equal(data) == EQUAL_RECORD)
        {
            removed[i] = true;
            cnt++;
        }
    }
    if(cnt == 0)
    {
        return SUCCEED;
    }
    int head = 0, tail = records.size() - 1;
    while(head < tail)
    {
        while(!removed[head] && head < tail)
        {
            head++;
        }
        if(head >= tail)
        {
            break;
        }
        while(removed[tail] && head < tail)
        {
            tail--;
        }
        if(head >= tail)
        {
            break;
        }
        DBRecordSlot::copy(records[tail], records[head], recordLength);
        head++;
        tail--;
    }
    pis->setFirstAvailableByte(pis->getFirstAvailableByte() - cnt * recordLength);
    int cur = pis->size(), end = pis->getFirstAvailableByte();
    records.clear();
    while(cur < end)
    {
        records.push_back(new DBRecordSlot((*this)[cur], ri));
        cur += recordLength;
    }
    return SUCCEED;
}

int DBDataPage::findEqual(std::map<int, void*>& data, std::set<std::map<std::string, void*>*>& result)
{
    for(int i = 0; i < records.size(); i++)
    {
        if(records[i]->equal(data) == EQUAL_RECORD)
        {
            std::map<std::string, void*>* re = new std::map<std::string, void*>();
            if(records[i]->read(*re) == SUCCEED)
            {
                result.insert(re);
            }
        }
    }
    return SUCCEED;
}

int DBDataPage::findEqual(std::map<int, void*>& data, std::set<char*>& result)
{
    for(int i = 0; i < records.size(); i++)
    {
        if(records[i]->equal(data) == EQUAL_RECORD)
        {
            char* _id = new char[DBType::typeSize(DBType::_ID)];
            records[i]->get_id(_id);
            result.insert(_id);
        }
    }
    return SUCCEED;
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
