#include"DBDataPage.h"

DBDataPage::DBDataPage(BufType cache, int index, int pageID, int mode, DBRecordInfo* ri):
    DBPage(cache, index, pageID, DBType::DATA_PAGE, mode), ri(ri)
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
            cur += ri->getRecordLength();
        }
    }
}

void DBDataPage::update(std::map<int, void*>& key_value, std::map<int, void*>& update_value)
{
    for(int i = 0; i < records.size(); i++)
    {
//        if(records[i]->equal(key_value))
//        {
//            records[i]->update(update_value);
//        }
    }
}

void DBDataPage::remove(std::map<int, void*>& data)
{
    std::vector<bool> removed;
    removed.assign(records.size(), false);
    int cnt = 0;
    for(int i = 0; i < records.size(); i++)
    {
//        if(records[i]->equal(data))
//        {
//            removed[i] = true;
//            cnt++;
//        }
    }
    if(cnt == 0)
    {
        return;
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
        DBRecordSlot::copy(records[tail], records[head], ri->getRecordLength());
        head++;
        tail--;
    }
    pis->setFirstAvailableByte(pis->getFirstAvailableByte() - cnt * ri->getRecordLength());
    int cur = pis->size(), end = pis->getFirstAvailableByte();
    records.clear();
    while(cur < end)
    {
        records.push_back(new DBRecordSlot((*this)[cur], ri));
        cur += ri->getRecordLength();
    }
}

void DBDataPage::findEqual(std::map<int, void*>& data, std::set<std::map<std::string, void*>*>& result)
{
//    for(int i = 0; i < records.size(); i++)
//    {
//        if(records[i]->equal(data))
//        {
//            std::map<std::string, void*>* re = new std::map<std::string, void*>();
//            records[i]->read(*re);
//            result.insert(re);
//        }
//    }
}

void DBDataPage::findEqual(std::map<int, void*>& data, std::set<char*>& result)
{
//    for(int i = 0; i < records.size(); i++)
//    {
//        if(records[i]->equal(data))
//        {
//            char* _id = new char[DBType::typeSize(DBType::_ID)];
//            records[i]->get_id(_id);
//            result.insert(_id);
//        }
//    }
}

void DBDataPage::filterByNull(std::map<int, bool>& nulls, std::list<std::vector<void*> >& datas)
{
    for(int i = 0; i < records.size(); i++)
    {
        records[i]->checkNull(nulls, datas);
    }
}

void DBDataPage::filterByValue(std::map<int, void*>& info, std::list<std::vector<void*> >& datas, int op)
{
    for(int i = 0; i < records.size(); i++)
    {
        records[i]->checkValue(info, datas, op);
    }
}

void DBDataPage::filterByFields(std::map<int, int>& info, std::list<std::vector<void*> >& datas, int op)
{
    for(int i = 0; i < records.size(); i++)
    {
        records[i]->checkFields(info, datas, op);
    }
}

void DBDataPage::print()
{
    DBPage::print();
    DBPrint::log("This page contains ").log(records.size()).logLine(" records.");
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
    slot->write(data);
    cur += ri->getRecordLength();
    pis->setFirstAvailableByte(cur);
    records.push_back(slot);
    if(cur + ri->getRecordLength() < PAGE_SIZE)
    {
        return SUCCEED;
    }
    return PAGE_FULL;
}
