#include"DBDataFile.h"

DBDataFile::DBDataFile(const char* root):
    root(root)
{
    fm = DBFileIOModel::getInstance();
    lastUsagePage = -1;
    lastDataPage = -1;
    open = false;
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
        DBPrintLine("=================");
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
    DBLog("Allocated new usage page ");
    DBLogLine(cnt);
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
    DBDataPage* dp = new DBDataPage(cache, index, cnt, dfdp->getRecordLength(), MODE_CREATE, ri);
    pages[cnt] = dp;
    DBLog("Allocated new data page ");
    DBLogLine(cnt);
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
    DBDataPage* re = new DBDataPage(cache, index, pid, dfdp->getRecordLength(), MODE_PARSE, ri);
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

int DBDataFile::createFile(const char* name)
{
    if(open)
    {
        DBLogLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->createFile(name) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    if(fm->openFile(name, fileID) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    ri = new DBRecordInfo();
    dfdp = new DBDataFileDescriptionPage(cache, index, 0, MODE_CREATE, ri);
    dfdp->addField("_id", DBType::_ID, false);
    fm->flush(dfdp->getIndex());
    fm->closeFile(fileID);
    delete ri;
    return SUCCEED;
}

int DBDataFile::deleteFile(const char* name)
{
    if(open)
    {
        DBLogLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->deleteFile(name) != SUCCEED)
    {
        DBLogLine("ERROR");
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
        DBLogLine("ERROR");
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

int DBDataFile::addField(const char* name, int type, bool nullable)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    int re = dfdp->addField(name, type, nullable);
    //TODO: might need to rewrite the whole file. But this is not included in the requirements seemingly, so leave it here for now.
    switch(re)
    {
    case EMPTY_FIELD_NAME:
        DBPrintLine("Field name cannot be empty.");
        break;
    case FIELD_ALREADY_EXIST:
        DBPrintLine("Field " + std::string(name) + " already exists.");
        break;
    case EXCEED_PAGE_LIMIT:
        DBPrintLine("You cannot add any more fields to this table.");
        break;
    }
    return re;
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
        if(!ri->indexes.count(iter->first))
        {
            errors[iter->first] = EXTRA_FIELD;
            continue;
        }
        int idx = ri->indexes[iter->first];
        if(idx == 0)
        {
            errors[iter->first] = EDIT__ID;
            continue;
        }
        if(!ri->nullables[idx] && iter->second == NULL)
        {
            errors[iter->first] = UNNULLABLE;
        }
        included[idx] = true;
        processed[idx] = iter->second;
    }
    for(int i = 1; i < included.size(); i++)
    {
        if(!included[i] && !ri->nullables[i])
        {
            errors[ri->names[i]] = UNNULLABLE;
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
        if(!ri->indexes.count(iter->first))
        {
            errors.push_back(iter->first);
            continue;
        }
        int idx = ri->indexes[iter->first];
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
            DBPrintLine("This table does not contain field " + errors[i]);
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
                DBPrintLine("This table does not contain field " + errors[i]);
            }
            return ERROR;
        }
    }
    else
    {
        for(int i = 0; i < errors.size(); i++)
        {
            DBPrintLine("This table does not contain field " + errors[i]);
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
        int i = 0;
        for(std::set<std::map<std::string, void*>*>::iterator iter = result.begin(); iter != result.end(); iter++)
        {
            std::map<std::string, void*>* ele = *iter;
            DBPrint("Equal record: ");
            DBPrintLine(i++);
            for(std::map<std::string, void*>::iterator iter2 = ele->begin(); iter2 != ele->end(); iter2++)
            {
                std::string name =iter2->first;
                int idx = ri->indexes[name];
                void* ptr = iter2->second;
                DBPrint(name + ": ");
                if(ptr == NULL)
                {
                    DBPrintLine("NULL");
                }
                else
                {
                    switch(ri->types[idx])
                    {
                    case DBType::_ID:
                        DBPrint_ID((char*)ptr);
                        DBPrintLine("");
                        break;
                    case DBType::INT:
                        DBPrintLine(*(int*)ptr);
                        break;
                    }
                }
            }
        }
        return SUCCEED;
    }
    else
    {
        for(int i = 0; i < errors.size(); i++)
        {
            DBPrintLine("This table does not contain field " + errors[i]);
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
                DBPrintLine("Field " + iter->first + " should be assigned.");
                break;
            case EXTRA_FIELD:
                DBPrintLine("This table does not contain field " + iter->first);
                break;
            case EDIT__ID:
                DBPrintLine("You should not modify _id of any record.");
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
        DBPrintLine("This field is already the primary key.");
        break;
    case FIELD_NOT_EXIST:
        DBPrintLine("This table does not contain field " + std::string(name));
        break;
    }
    return re;
}

int DBDataFile::openFile(const char* name)
{
    if(open)
    {
        DBLogLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->openFile(name, fileID) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    ri = new DBRecordInfo();
    dfdp = new DBDataFileDescriptionPage(cache, index, 0, MODE_PARSE, ri);
    open = true;
    return SUCCEED;
}


