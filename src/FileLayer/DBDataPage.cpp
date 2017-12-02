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

int DBDataPage::update(SearchInfo& si, UpdateInfo& ui)
{
    int cnt = 0;
    for(int i = 0; i < records.size(); i++)
    {
        if(!records[i]->checkNull(si.nulls))
        {
            continue;
        }
        bool flag = true;
        for(int j = 0; j < 6; j++)
        {
            if(!records[i]->checkValue(si.values[j], j) || !records[i]->checkFields(si.fields[j], j))
            {
                flag = false;
                break;
            }
        }
        if(flag)
        {
            records[i]->update(ui);
            cnt++;
        }
    }
    return cnt;
}

int DBDataPage::remove(SearchInfo& si)
{
    std::vector<bool> removed;
    removed.assign(records.size(), false);
    int cnt = 0;
    for(int i = 0; i < records.size(); i++)
    {
        if(!records[i]->checkNull(si.nulls))
        {
            continue;
        }
        bool flag = true;
        for(int j = 0; j < 6; j++)
        {
            if(!records[i]->checkValue(si.values[j], j) || !records[i]->checkFields(si.fields[j], j))
            {
                flag = false;
                break;
            }
        }
        if(flag)
        {
            cnt++;
            removed[i] = true;
        }
    }
    if(cnt == 0)
    {
        return 0;
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
    return cnt;
}

void DBDataPage::filterByNull(std::map<int, bool>& nulls, std::list<std::vector<void*> >& datas)
{
    for(int i = 0; i < records.size(); i++)
    {
        if(records[i]->checkNull(nulls))
        {
            datas.push_back(std::vector<void*>());
            records[i]->read(datas.back());
        }
    }
}

void DBDataPage::filterByValue(std::map<int, vector<void*> >& info, std::list<std::vector<void*> >& datas, int op)
{
    for(int i = 0; i < records.size(); i++)
    {
        if(records[i]->checkValue(info, op))
        {
            datas.push_back(std::vector<void*>());
            records[i]->read(datas.back());
        }
    }
}

void DBDataPage::filterByFields(std::map<int, vector<int> >& info, std::list<std::vector<void*> >& datas, int op)
{
    for(int i = 0; i < records.size(); i++)
    {
        if(records[i]->checkFields(info, op))
        {
            datas.push_back(std::vector<void*>());
            records[i]->read(datas.back());
        }
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
