#include"DataFile.h"

DataFile::DataFile(std::string path):
    path(path)
{
    fm = FileIOModel::getInstance();
    lastUsagePage = -1;
    lastDataPage = -1;
    open = false;
    ri = new RecordInfo();
    buffer = new char[8192];
}

RecordInfo* DataFile::getRecordInfo()
{
    return ri;
}

void DataFile::printAllRecords()
{
    assert(open);
    DataPage* dp = openDataPage(dfdp->getFirstDataPage());
    while(true)
    {
        if(dp == NULL)
        {
            return;
        }
        dp->print();
        dp->printAllRecords();
        Print::printLine("====================");
        dp = openDataPage(dp->getNextSameType());
    }
}

int DataFile::findFirstAvailableDataPage()
{
    assert(open);
    UsagePage* up = openUsagePage(dfdp->getFirstUsagePage());
    while(true)
    {
        if(up == NULL)
        {
            return allocateNewDataPage();
        }
        int re = up->findFirstAvailable();
        if(re > 0 && re < dfdp->getPageNumber())
        {
            return re;
        }
        up = openUsagePage(up->getNextSameType());
    }
}

void DataFile::setAvailableOfDataPage(int dpid, bool available)
{
    assert(open);
    UsagePage* up = openUsagePage(dfdp->getFirstUsagePage());
    while(true)
    {
        if(up == NULL)
        {
            return;
        }
        if(up->withinRange(dpid))
        {
            break;
        }
        up = openUsagePage(up->getNextSameType());
    }
    up->setAvailable(dpid, available);
}

int DataFile::allocateNewUsagePage()
{
    assert(open);
    int cnt = dfdp->getPageNumber();
    if(cnt <= 0)
    {
        throw Exception("Invalid page number.");
    }
    int index;
    BufType cache = fm->getPage(fileID, cnt, index);
    UsagePage* up = new UsagePage(cache, index, cnt, false);
    pages[cnt] = up;
    Print::log("Allocated new usage page ").logLine(cnt);
    dfdp->incrementPageNumber(Type::USAGE_PAGE);
    UsagePage* last;
    if((last = openUsagePage(lastUsagePage)) != NULL)
    {
        last->setNextSameType(cnt);
    }
    lastUsagePage = cnt;
    return cnt;
}

int DataFile::allocateNewDataPage()
{
    assert(open);
    int cnt = dfdp->getPageNumber();
    if(cnt <= 0)
    {
        throw Exception("Invalid page number.");
    }
    UsagePage* up = openUsagePage(lastUsagePage);
    if(up == NULL || !up->withinRange(cnt))
    {
        up = openUsagePage(allocateNewUsagePage());
    }
    cnt = dfdp->getPageNumber();
    int index;
    BufType cache = fm->getPage(fileID, cnt, index);
    DataPage* dp = new DataPage(cache, index, cnt, false, ri);
    pages[cnt] = dp;
    Print::log("Allocated new data page ").logLine(cnt);
    dfdp->incrementPageNumber(Type::DATA_PAGE);
    DataPage* last;
    if((last = openDataPage(lastDataPage)) != NULL)
    {
        last->setNextSameType(cnt);
    }
    lastDataPage = cnt;
    if(!up->withinRange(cnt))
    {
        throw Exception("Internal error when allocating new data page.");
    }
    up->setAvailable(cnt, true);
    return cnt;
}

DataPage* DataFile::openDataPage(int pid)
{
    assert(open);
    if(pages.count(pid))
    {
        Page* re = pages[pid];
        return re->getPageType() == Type::DATA_PAGE ? (DataPage*)re : NULL;
    }
    if(pid <= 0 || pid >= dfdp->getPageNumber())
    {
        return NULL;
    }
    int index;
    BufType cache = fm->getPage(fileID, pid, index);
    if(PageInfoSlot::getPageType(cache) != Type::DATA_PAGE)
    {
        return NULL;
    }
    DataPage* re = new DataPage(cache, index, pid, true, ri);
    pages[pid] = re;
    return re;
}

UsagePage* DataFile::openUsagePage(int pid)
{
    assert(open);
    if(pages.count(pid))
    {
        Page* re = pages[pid];
        return re->getPageType() == Type::USAGE_PAGE ? (UsagePage*)re : NULL;
    }
    if(pid <= 0 || pid >= dfdp->getPageNumber())
    {
        return NULL;
    }
    int index;
    BufType cache = fm->getPage(fileID, pid, index);
    if(PageInfoSlot::getPageType(cache) != Type::USAGE_PAGE)
    {
        return NULL;
    }
    UsagePage* re = new UsagePage(cache, index, pid, true);
    pages[pid] = re;
    return re;
}

void DataFile::createFile()
{
    assert(!open);
    fm->createFile(path.c_str());
    fm->openFile(path.c_str(), fileID);
    int index;
    open = true;
    ri->init();
    BufType cache = fm->getPage(fileID, 0, index);
    dfdp = new DataFileDescPage(cache, index, 0, false, ri);
    ri->addField("_id", Type::_ID, false, 0);
    dfdp->writeFields();
    fm->flush(dfdp->getIndex());
    fm->closeFile(fileID);
    open = false;
}

void DataFile::deleteFile()
{
    assert(!open);
    fm->deleteFile(path.c_str());
}

void DataFile::closeFile()
{
    assert(open);
    fm->flush(dfdp->getIndex());
    for(std::map<int, Page*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
    {
        fm->flush(iter->second->getIndex());
    }
    pages.clear();
    open = false;
    fm->closeFile(fileID);
}

void DataFile::printFileDesc()
{
    assert(open);
    dfdp->print();
}

void DataFile::printRecordDesc()
{
    assert(open);
    int cnt = ri->getFieldCount();
    std::ostringstream oss;
    for(int i = 1; i < cnt; i++)
    {
        oss << "    " << ri->name(i) << " " << Type::typeName(ri->type(i));
        if(ri->extra(i) > 0)
        {
            oss << "(" << ri->extra(i) << ")";
        }
        if(!ri->nullable(i))
        {
            oss << " NOT NULL";
        }
        if(i != cnt - 1)
        {
            oss << ",";
        }
        oss << "\n";
    }
    Print::print(oss.str());
}

void DataFile::addFields(std::vector<std::string>& name, std::vector<int>& type,
                          std::vector<bool>& nullable, std::vector<int>& extra)
{
    assert(open);
    if(name.size() != type.size() ||
       type.size() != nullable.size() ||
       nullable.size() != extra.size())
    {
        throw Exception("Numbers of parameters passed into addFields don't match.");
    }
    for(int i = 0; i < name.size(); i++)
    {
        int re = ri->addField(name[i], type[i], nullable[i], extra[i]);
        //TODO: might need to rewrite the whole file. But this is not included in the requirements seemingly, so leave it here for now.
        switch(re)
        {
        case RecordInfo::EMPTY_FIELD_NAME:
            ri->reset(i);
            throw Exception("Field name cannot be empty.");
        case RecordInfo::FIELD_ALREADY_EXIST:
            ri->reset(i);
            throw Exception("Field " + name[i] + " already exists.");
        case RecordInfo::EXCEED_PAGE_LIMIT:
            ri->reset(i);
            throw Exception("You cannot add any more fields to this table.");
        }
    }
    dfdp->writeFields();
}

int DataFile::remove(SearchInfo& si)
{
    assert(open);
    DataPage* dp = openDataPage(dfdp->getFirstDataPage());
    int cnt = 0;
    while(true)
    {
        if(dp == NULL)
        {
            break;
        }
        cnt += dp->remove(si);
        dp = openDataPage(dp->getNextSameType());
    }
    return cnt;
}

int DataFile::update(SearchInfo& si, UpdateInfo& ui)
{
    assert(open);
    DataPage* dp = openDataPage(dfdp->getFirstDataPage());
    int cnt = 0;
    while(true)
    {
        if(dp == NULL)
        {
            break;
        }
        cnt += dp->update(si, ui);
        dp = openDataPage(dp->getNextSameType());
    }
    return cnt;
}

void DataFile::select(SearchInfo& si, SelectResult& sr)
{
    assert(open);
    DataPage* dp = openDataPage(dfdp->getFirstDataPage());
    while(true)
    {
        if(dp == NULL)
        {
            break;
        }
        dp->select(si, sr);
        dp = openDataPage(dp->getNextSameType());
    }
}

bool DataFile::validateInsertion(std::vector<void*>& data)
{
    assert(open);
    int n = ri->getFieldCount();
    if(n != data.size())
    {
        Print::printLine("Not enough fields provided.");
        return false;
    }
    bool flag = true;
    for(int i = 1; i < data.size(); i++)
    {
        if(!ri->nullable(i) && data[i] == NULL)
        {
            flag = false;
            Print::print("Field ").print(ri->name(i)).printLine(" cannot be null");
        }
    }
    return flag;
}

void DataFile::insert(std::vector<void*>& fields)
{
    assert(open);
    if(!validateInsertion(fields))
    {
        return;
    }
    int fadp = findFirstAvailableDataPage();
    int re = openDataPage(fadp)->insert(fields);
    if(re == DataPage::PAGE_FULL)
    {
        setAvailableOfDataPage(fadp, false);
    }
}

void DataFile::setPrimaryKey(const char* name)
{
//    assert(open);
//    dfdp->setPrimaryKey(name);
}

void DataFile::openFile()
{
    assert(!open);
    fm->openFile(path.c_str(), fileID);
    int index;
    open = true;
    ri->init();
    BufType cache = fm->getPage(fileID, 0, index);
    dfdp = new DataFileDescPage(cache, index, 0, true, ri);
}
