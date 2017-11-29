#include"DBDataFile.h"

DBDataFile::DBDataFile(std::string path):
    path(path)
{
    fm = DBFileIOModel::getInstance();
    lastUsagePage = -1;
    lastDataPage = -1;
    open = false;
    ri = new DBRecordInfo();
}

void DBDataFile::printAllRecords()
{
    if(!open)
    {
        return;
    }
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
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
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

int DBDataFile::setAvailableOfDataPage(int dpid, bool available)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    DBUsagePage* up = openUsagePage(dfdp->getFirstUsagePage());
    while(true)
    {
        if(up == NULL)
        {
            return SUCCEED;
        }
        if(up->withinRange(dpid))
        {
            break;
        }
        up = openUsagePage(up->getNextSameType());
    }
    up->setAvailable(dpid, available);
    return SUCCEED;
}

int DBDataFile::allocateNewUsagePage()
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    int cnt = dfdp->getPageNumber();
    if(cnt <= 0)
    {
        return ERROR;
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
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    int cnt = dfdp->getPageNumber();
    if(cnt <= 0)
    {
        return ERROR;
    }
    DBUsagePage* up = openUsagePage(lastUsagePage);
    if(up == NULL || !up->withinRange(cnt))
    {
        up = openUsagePage(allocateNewUsagePage());
        if(up == NULL)
        {
            return ERROR;
        }
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
        return ERROR;
    }
    up->setAvailable(cnt, true);
    return cnt;
}

DBDataPage* DBDataFile::openDataPage(int pid)
{
    if(!open)
    {
        return NULL;
    }
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
    if(!open)
    {
        return NULL;
    }
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

int DBDataFile::createFile()
{
    if(open)
    {
        DBPrint::logLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->createFile(path.c_str()) != SUCCEED)
    {
        DBPrint::logLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    if(fm->openFile(path.c_str(), fileID) != SUCCEED)
    {
        DBPrint::logLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
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
    return SUCCEED;
}

int DBDataFile::deleteFile()
{
    if(open)
    {
        DBPrint::logLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->deleteFile(path.c_str()) != SUCCEED)
    {
        DBPrint::logLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    return SUCCEED;
}

int DBDataFile::closeFile()
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    fm->flush(dfdp->getIndex());
    for(std::map<int, DBPage*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
    {
        fm->flush(iter->second->getIndex());
    }
    if(fm->closeFile(fileID) != SUCCEED)
    {
        DBPrint::logLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    pages.clear();
    open = false;
    return SUCCEED;
}

void DBDataFile::printFileDescription()
{
    if(!open)
    {
        return;
    }
    dfdp->print();
}

void DBDataFile::printRecordDescription()
{
    if(!open)
    {
        return;
    }
    dfdp->printRecordDescription();
}

int DBDataFile::addFields(std::vector<std::string>& name, std::vector<int>& type,
                          std::vector<bool>& nullable, std::vector<int>& extra)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    if(name.size() != type.size() ||
       type.size() != nullable.size() ||
       nullable.size() != extra.size())
    {
        return ERROR;
    }
    for(int i = 0; i < name.size(); i++)
    {
        int re = ri->addField(name[i], type[i], nullable[i], extra[i]);
        //TODO: might need to rewrite the whole file. But this is not included in the requirements seemingly, so leave it here for now.
        switch(re)
        {
        case EMPTY_FIELD_NAME:
            DBPrint::printLine("Field name cannot be empty.");
            ri->reset(i);
            return re;
        case FIELD_ALREADY_EXIST:
            DBPrint::printLine("Field " + name[i] + " already exists.");
            ri->reset(i);
            return re;
        case EXCEED_PAGE_LIMIT:
            DBPrint::printLine("You cannot add any more fields to this table.");
            ri->reset(i);
            return re;
        }
    }
    dfdp->writeFields();
    printFileDescription();
    return SUCCEED;
}

void DBDataFile::processWriteValue(std::map<std::string, void*>& data,
                                    std::vector<void*>& processed,
                                    std::map<std::string, int>& errors)
{
    if(!open)
    {
        return;
    }
    int n = ri->getFieldCount();
    std::vector<bool> included;
    included.assign(n, false);
    processed.assign(n, NULL);
    errors.clear();
    std::map<std::string, void*>::iterator iter;
    for(iter = data.begin(); iter != data.end(); iter++)
    {
        int idx = ri->index(iter->first);
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
        if(!ri->nullable(idx) && iter->second == NULL)
        {
            errors[iter->first] = UNNULLABLE;
        }
        included[idx] = true;
        processed[idx] = iter->second;
    }
    for(int i = 1; i < included.size(); i++)
    {
        if(!included[i] && !ri->nullable(i))
        {
            errors[ri->name(i)] = UNNULLABLE;
        }
    }
}

void DBDataFile::processKeyValue(std::map<std::string, void*>& data,
                                std::map<int, void*>& processed,
                                std::vector<std::string>& errors)
{
    if(!open)
    {
        return;
    }
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

int DBDataFile::remove(std::map<std::string, void*>& data)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
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
        return SUCCEED;
    }
    else
    {
        for(int i = 0; i < errors.size(); i++)
        {
            DBPrint::printLine("This table does not contain field " + errors[i]);
        }
        return ERROR;
    }
}

int DBDataFile::update(std::map<std::string, void*>& key_value, std::map<std::string, void*>& update_value)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
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
            return SUCCEED;
        }
        else
        {
            for(int i = 0; i < errors.size(); i++)
            {
                DBPrint::printLine("This table does not contain field " + errors[i]);
            }
            return ERROR;
        }
    }
    else
    {
        for(int i = 0; i < errors.size(); i++)
        {
            DBPrint::printLine("This table does not contain field " + errors[i]);
        }
        return ERROR;
    }
}

int DBDataFile::findEqual(std::map<std::string, void*>& data, std::set<std::map<std::string, void*>*>& result)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
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
//        int i = 0;
//        for(std::set<std::map<std::string, void*>*>::iterator iter = result.begin(); iter != result.end(); iter++)
//        {
//            std::map<std::string, void*>* ele = *iter;
//            DBPrint("Equal record: ");
//            DBPrintLine(i++);
//            for(std::map<std::string, void*>::iterator iter2 = ele->begin(); iter2 != ele->end(); iter2++)
//            {
//                std::string name =iter2->first;
//                int idx = ri->indexes[name];
//                void* ptr = iter2->second;
//                DBPrint(name + ": ");
//                if(ptr == NULL)
//                {
//                    DBPrintLine("NULL");
//                }
//                else
//                {
//                    switch(ri->types[idx])
//                    {
//                    case DBType::_ID:
//                        DBPrint_ID((char*)ptr);
//                        DBPrintLine("");
//                        break;
//                    case DBType::INT:
//                        DBPrintLine(*(int*)ptr);
//                        break;
//                    }
//                }
//            }
//        }
        return SUCCEED;
    }
    else
    {
        for(int i = 0; i < errors.size(); i++)
        {
            DBPrint::printLine("This table does not contain field " + errors[i]);
        }
        return ERROR;
    }
}

int DBDataFile::insertRecordToPage(int page, std::vector<void*>& processed)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    DBDataPage* dp = (DBDataPage*)(pages[page]);
    return dp->insert(processed);
}

int DBDataFile::insertRecord(std::map<std::string, void*>& fields)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    std::vector<void*> processed;
    std::map<std::string, int> errors;
    processWriteValue(fields, processed, errors);
    if(errors.empty())
    {
        int fadp = findFirstAvailableDataPage();
        int re = insertRecordToPage(fadp, processed);
        switch(re)
        {
        case ERROR:
            return ERROR;
        case DATA_PAGE_FULL:
            setAvailableOfDataPage(fadp, false);
        }
    }
    else
    {
        std::map<std::string, int>::iterator iter;
        for(iter = errors.begin(); iter != errors.end(); iter++)
        {
            switch(iter->second)
            {
            case UNNULLABLE:
                DBPrint::printLine("Field " + iter->first + " should be assigned.");
                break;
            case EXTRA_FIELD:
                DBPrint::printLine("This table does not contain field " + iter->first);
                break;
            case EDIT__ID:
                DBPrint::printLine("You should not modify _id of any record.");
                break;
            }
        }
        return ERROR;
    }
    return SUCCEED;
}

int DBDataFile::setPrimaryKey(const char* name)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    int re = dfdp->setPrimaryKey(name);
    switch(re)
    {
    case FIELD_IS_ALREADY_PRIMARY_KEY:
        DBPrint::printLine("This field is already the primary key.");
        break;
    case FIELD_NOT_EXIST:
        DBPrint::printLine("This table does not contain field " + std::string(name));
        break;
    }
    return re;
}

int DBDataFile::openFile()
{
    if(open)
    {
        DBPrint::logLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->openFile(path.c_str(), fileID) != SUCCEED)
    {
        DBPrint::logLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    int index;
    open = true;
    ri->init();
    BufType cache = fm->getPage(fileID, 0, index);
    dfdp = new DBDataFileDescriptionPage(cache, index, 0, MODE_PARSE, ri);
    return SUCCEED;
}

void DBDataFile::_test()
{
    createFile();
    openFile();
//    setPrimaryKey("_id");
//    setPrimaryKey("test1");
    int data = 1;
    int data2 = 99999;
    int data3 = -99999;
    int data4 = 10;
//    int i = 0;
//    do
//    {
//        printFileDescription();
//        DBLogLine("=========================================");
//        sprintf(name, "test%d", i++);
//    }while(addField(name, DBType::INT, true) == SUCCEED);
    std::vector<std::string> names;
    std::vector<int> types;
    std::vector<bool> nullables;
    std::vector<int> extras;
    names.push_back("test1");
    names.push_back("test2");
    names.push_back("test3");
    types.push_back(1);
    types.push_back(1);
    types.push_back(1);
    nullables.push_back(false);
    nullables.push_back(false);
    nullables.push_back(false);
    extras.push_back(0);
    extras.push_back(5);
    extras.push_back(30);
    addFields(names, types, nullables, extras);
    printFileDescription();
    map<string, void*> f, f2, f3;
    f["test1"] = &data;
    f["test2"] = &data2;
    f["test3"] = &data3;
    for(int i = 0; i < 1000; i++)
        insertRecord(f);
//    f2["test2"] = &data4;
//    update(f, f2);
//    f3["test2"] = &data4;
//    printAllRecords();
    closeFile();
    openFile();
    printFileDescription();
    printAllRecords();
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
