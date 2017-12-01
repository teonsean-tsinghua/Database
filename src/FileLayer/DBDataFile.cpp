#include"DBDataFile.h"

SelectResult::SelectResult()
{

}

DBDataFile::DBDataFile(std::string path):
    path(path)
{
    fm = DBFileIOModel::getInstance();
    lastUsagePage = -1;
    lastDataPage = -1;
    open = false;
    ri = new DBRecordInfo();
}

DBRecordInfo* DBDataFile::getRecordInfo()
{
    return ri;
}

void DBDataFile::printAllRecords()
{
    assert(open);
    DBDataPage* dp = openDataPage(dfdp->getFirstDataPage());
    while(true)
    {
        if(dp == NULL)
        {
            return;
        }
        dp->print();
        dp->printAllRecords();
        DBPrint::printLine("====================");
        dp = openDataPage(dp->getNextSameType());
    }
}

int DBDataFile::findFirstAvailableDataPage()
{
    assert(open);
    DBUsagePage* up = openUsagePage(dfdp->getFirstUsagePage());
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

void DBDataFile::setAvailableOfDataPage(int dpid, bool available)
{
    assert(open);
    DBUsagePage* up = openUsagePage(dfdp->getFirstUsagePage());
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

int DBDataFile::allocateNewUsagePage()
{
    assert(open);
    int cnt = dfdp->getPageNumber();
    if(cnt <= 0)
    {
        throw Exception("Invalid page number.");
    }
    int index;
    BufType cache = fm->getPage(fileID, cnt, index);
    DBUsagePage* up = new DBUsagePage(cache, index, cnt, MODE_CREATE);
    pages[cnt] = up;
    DBPrint::log("Allocated new usage page ").logLine(cnt);
    dfdp->incrementPageNumber(DBType::USAGE_PAGE);
    DBUsagePage* last;
    if((last = openUsagePage(lastUsagePage)) != NULL)
    {
        last->setNextSameType(cnt);
    }
    lastUsagePage = cnt;
    return cnt;
}

int DBDataFile::allocateNewDataPage()
{
    assert(open);
    int cnt = dfdp->getPageNumber();
    if(cnt <= 0)
    {
        throw Exception("Invalid page number.");
    }
    DBUsagePage* up = openUsagePage(lastUsagePage);
    if(up == NULL || !up->withinRange(cnt))
    {
        up = openUsagePage(allocateNewUsagePage());
    }
    cnt = dfdp->getPageNumber();
    int index;
    BufType cache = fm->getPage(fileID, cnt, index);
    DBDataPage* dp = new DBDataPage(cache, index, cnt, MODE_CREATE, ri);
    pages[cnt] = dp;
    DBPrint::log("Allocated new data page ").logLine(cnt);
    dfdp->incrementPageNumber(DBType::DATA_PAGE);
    DBDataPage* last;
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

DBDataPage* DBDataFile::openDataPage(int pid)
{
    assert(open);
    if(pages.count(pid))
    {
        DBPage* re = pages[pid];
        return re->getPageType() == DBType::DATA_PAGE ? (DBDataPage*)re : NULL;
    }
    if(pid <= 0 || pid >= dfdp->getPageNumber())
    {
        return NULL;
    }
    int index;
    BufType cache = fm->getPage(fileID, pid, index);
    if(DBPageInfoSlot::getPageType(cache) != DBType::DATA_PAGE)
    {
        return NULL;
    }
    DBDataPage* re = new DBDataPage(cache, index, pid, MODE_PARSE, ri);
    pages[pid] = re;
    return re;
}

DBUsagePage* DBDataFile::openUsagePage(int pid)
{
    assert(open);
    if(pages.count(pid))
    {
        DBPage* re = pages[pid];
        return re->getPageType() == DBType::USAGE_PAGE ? (DBUsagePage*)re : NULL;
    }
    if(pid <= 0 || pid >= dfdp->getPageNumber())
    {
        return NULL;
    }
    int index;
    BufType cache = fm->getPage(fileID, pid, index);
    if(DBPageInfoSlot::getPageType(cache) != DBType::USAGE_PAGE)
    {
        return NULL;
    }
    DBUsagePage* re = new DBUsagePage(cache, index, pid, MODE_PARSE);
    pages[pid] = re;
    return re;
}

void DBDataFile::createFile()
{
    assert(!open);
    fm->createFile(path.c_str());
    fm->openFile(path.c_str(), fileID);
    int index;
    open = true;
    ri->init();
    BufType cache = fm->getPage(fileID, 0, index);
    dfdp = new DBDataFileDescriptionPage(cache, index, 0, MODE_CREATE, ri);
    ri->addField("_id", DBType::_ID, false, 0);
    dfdp->writeFields();
    fm->flush(dfdp->getIndex());
    fm->closeFile(fileID);
    open = false;
}

void DBDataFile::deleteFile()
{
    assert(!open);
    fm->deleteFile(path.c_str());
}

void DBDataFile::closeFile()
{
    assert(open);
    fm->flush(dfdp->getIndex());
    for(std::map<int, DBPage*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
    {
        fm->flush(iter->second->getIndex());
    }
    pages.clear();
    open = false;
    fm->closeFile(fileID);
}

void DBDataFile::printFileDescription()
{
    assert(open);
    dfdp->print();
}

void DBDataFile::printRecordDescription()
{
    assert(open);
    dfdp->printRecordDescription();
}

void DBDataFile::addFields(std::vector<std::string>& name, std::vector<int>& type,
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
        case DBRecordInfo::EMPTY_FIELD_NAME:
            ri->reset(i);
            throw Exception("Field name cannot be empty.");
        case DBRecordInfo::FIELD_ALREADY_EXIST:
            ri->reset(i);
            throw Exception("Field " + name[i] + " already exists.");
        case DBRecordInfo::EXCEED_PAGE_LIMIT:
            ri->reset(i);
            throw Exception("You cannot add any more fields to this table.");
        }
    }
    dfdp->writeFields();
}

void DBDataFile::processKeyValue(std::map<std::string, void*>& data,
                                std::map<int, void*>& processed,
                                std::vector<std::string>& errors)
{
    assert(open);
    errors.clear();
    std::map<std::string, void*>::iterator iter;
    for(iter = data.begin(); iter != data.end(); iter++)
    {
        int idx = ri->index(iter->first);
        if(idx < 0)
        {
            errors.push_back(iter->first);
            continue;
        }
        processed[idx] = iter->second;
    }
}

void DBDataFile::remove(std::map<std::string, void*>& data)
{
    assert(open);
    std::map<int, void*> processed;
    std::vector<std::string> errors;
    processKeyValue(data, processed, errors);
    if(errors.empty())
    {
        DBDataPage* dp = openDataPage(dfdp->getFirstDataPage());
        while(true)
        {
            if(dp == NULL)
            {
                break;
            }
            dp->remove(processed);
            dp = openDataPage(dp->getNextSameType());
        }
    }
    else
    {
        for(int i = 0; i < errors.size(); i++)
        {
            DBPrint::printLine("This table does not contain field " + errors[i]);
        }
    }
}

void DBDataFile::update(std::map<std::string, void*>& key_value, std::map<std::string, void*>& update_value)
{
    assert(open);
    std::map<int, void*> processed;
    std::vector<std::string> errors;
    processKeyValue(key_value, processed, errors);
    if(errors.empty())
    {
        std::map<int, void*> processed2;
        std::vector<std::string> errors2;
        processKeyValue(update_value, processed2, errors2);
        if(errors2.empty())
        {
            DBDataPage* dp = openDataPage(dfdp->getFirstDataPage());
            while(true)
            {
                if(dp == NULL)
                {
                    break;
                }
                dp->update(processed, processed2);
                dp = openDataPage(dp->getNextSameType());
            }
        }
        else
        {
            for(int i = 0; i < errors.size(); i++)
            {
                DBPrint::printLine("This table does not contain field " + errors[i]);
            }
        }
    }
    else
    {
        for(int i = 0; i < errors.size(); i++)
        {
            DBPrint::printLine("This table does not contain field " + errors[i]);
        }
    }
}

void DBDataFile::select(SearchInfo& si, SelectResult& sr)
{
    assert(open);
    printAllRecords();
    if(si.nulls.size() > 0)
    {
        DBDataPage* dp = openDataPage(dfdp->getFirstDataPage());
        while(true)
        {
            if(dp == NULL)
            {
                break;
            }
            dp->filterByNull(si.nulls, sr.results);
            dp = openDataPage(dp->getNextSameType());
        }
        DBPrint::printLine(sr.results.size());
    }
    if(si.equals.size() > 0)
    {
        if(sr.results.empty())
        {

        }
        else
        {

        }
    }
    if(si.notEquals.size() > 0)
    {
        if(sr.results.empty())
        {

        }
        else
        {

        }
    }
    if(si.notEquals.size() > 0)
    {
        if(sr.results.empty())
        {

        }
        else
        {

        }
    }
}

void DBDataFile::findEqual(std::map<std::string, void*>& data, std::set<std::map<std::string, void*>*>& result)
{
    assert(open);
    std::map<int, void*> processed;
    std::vector<std::string> errors;
    processKeyValue(data, processed, errors);
    if(errors.empty())
    {
        DBDataPage* dp = openDataPage(dfdp->getFirstDataPage());
        while(true)
        {
            if(dp == NULL)
            {
                break;
            }
            dp->findEqual(processed, result);
            dp = openDataPage(dp->getNextSameType());
        }
    }
    else
    {
        for(int i = 0; i < errors.size(); i++)
        {
            DBPrint::printLine("This table does not contain field " + errors[i]);
        }
    }
}

bool DBDataFile::validateInsertion(std::vector<void*>& data)
{
    assert(open);
    int n = ri->getFieldCount();
    if(n != data.size())
    {
        DBPrint::printLine("Not enough fields provided.");
        return false;
    }
    bool flag = true;
    for(int i = 1; i < data.size(); i++)
    {
        if(!ri->nullable(i) && data[i] == NULL)
        {
            flag = false;
            DBPrint::print("Field ").print(ri->name(i)).printLine(" cannot be null");
        }
    }
    return flag;
}

void DBDataFile::insert(std::vector<void*>& fields)
{
    assert(open);
    if(!validateInsertion(fields))
    {
        return;
    }
    int fadp = findFirstAvailableDataPage();
    int re = openDataPage(fadp)->insert(fields);
    if(re == DBDataPage::PAGE_FULL)
    {
        setAvailableOfDataPage(fadp, false);
    }
}

void DBDataFile::setPrimaryKey(const char* name)
{
    assert(open);
    dfdp->setPrimaryKey(name);
}

void DBDataFile::openFile()
{
    assert(!open);
    fm->openFile(path.c_str(), fileID);
    int index;
    open = true;
    ri->init();
    BufType cache = fm->getPage(fileID, 0, index);
    dfdp = new DBDataFileDescriptionPage(cache, index, 0, MODE_PARSE, ri);
}

void DBDataFile::_test()
{
//    createFile();
//    openFile();
//    setPrimaryKey("_id");
//    setPrimaryKey("test1");
//    int data = 1;
//    int data2 = 99999;
//    int data3 = -99999;
//    int data4 = 10;
//    int i = 0;
//    do
//    {
//        printFileDescription();
//        DBLogLine("=========================================");
//        sprintf(name, "test%d", i++);
//    }while(addField(name, DBType::INT, true) == SUCCEED);
//    std::vector<std::string> names;
//    std::vector<int> types;
//    std::vector<bool> nullables;
//    std::vector<int> extras;
//    names.push_back("test1");
//    names.push_back("test2");
//    names.push_back("test3");
//    types.push_back(1);
//    types.push_back(1);
//    types.push_back(1);
//    nullables.push_back(false);
//    nullables.push_back(false);
//    nullables.push_back(false);
//    extras.push_back(0);
//    extras.push_back(5);
//    extras.push_back(30);
//    addFields(names, types, nullables, extras);
//    printFileDescription();
//    map<string, void*> f, f2, f3;
//    f["test1"] = &data;
//    f["test2"] = &data2;
//    f["test3"] = &data3;
//    for(int i = 0; i < 1000; i++)
//        insertRecord(f);
//    f2["test2"] = &data4;
//    update(f, f2);
//    f3["test2"] = &data4;
//    printAllRecords();
//    closeFile();
//    openFile();
//    printFileDescription();
//    printAllRecords();
//    set<map<string, void*>*> re;
//    findEqual(f, re);
//    remove(f);
//    printAllRecords();
}

void DBDataFile::test()
{
    DBDataFile df("test.dat");
    df._test();
}
