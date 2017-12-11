#include"DataPage.h"

DataPage::DataPage(BufType cache, int index, int pageID, bool parse, RecordInfo* ri):
    Page(cache, index, pageID, Type::DATA_PAGE), ri(ri)
{
    if(!parse)
    {
        pis->setPageType(Type::DATA_PAGE);
        pis->setNextSamePage(-1);
        pis->setLengthFixed(true);
        pis->setFirstAvailableByte(pis->size());
        records.clear();
    }
    else
    {
        int cur = pis->size(), end = pis->getFirstAvailableByte();
        while(cur < end)
        {
            records.push_back(new RecordSlot((*this)[cur], ri));
            cur += ri->getRecordLength();
        }
    }
}

int DataPage::update(SearchInfo& si, UpdateInfo& ui)
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

int DataPage::remove(SearchInfo& si)
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
        RecordSlot::copy(records[tail], records[head], ri->getRecordLength());
        head++;
        tail--;
    }
    pis->setFirstAvailableByte(pis->getFirstAvailableByte() - cnt * ri->getRecordLength());
    int cur = pis->size(), end = pis->getFirstAvailableByte();
    records.clear();
    while(cur < end)
    {
        records.push_back(new RecordSlot((*this)[cur], ri));
        cur += ri->getRecordLength();
    }
    return cnt;
}

void DataPage::filterByNull(std::map<int, bool>& nulls, std::list<std::vector<void*> >& datas)
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

void DataPage::filterByValue(std::map<int, vector<void*> >& info, std::list<std::vector<void*> >& datas, int op)
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

void DataPage::filterByFields(std::map<int, vector<int> >& info, std::list<std::vector<void*> >& datas, int op)
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

void DataPage::print()
{
    Page::print();
    Print::log("This page contains ").log(records.size()).logLine(" records.");
}

void DataPage::printAllRecords()
{
    for(int i = 0; i < records.size(); i++)
    {
        records[i]->print();
    }
}

int DataPage::insert(std::vector<void*>& data)
{
    int cur = pis->getFirstAvailableByte();
    RecordSlot* slot = new RecordSlot((*this)[cur], ri);
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
