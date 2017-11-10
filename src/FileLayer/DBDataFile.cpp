#include"DBDataFile.h"

DBDataFile::DBDataFile(const char* root):
    root(root)
{
    fm = DBFileIOModel::getInstance();
    lastUsagePage = -1;
    lastDataPage = -1;
}

int DBDataFile::findFirstAvailableDataPage()
{
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
    DBUsagePage* up = openUsagePage(dfdp->getFirstUsagePage());
    while(true)
    {
        if(up == NULL)
        {
            return ERROR;
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
}

int DBDataFile::deleteFile(const char* name)
{
    if(fm->deleteFile(name) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
}

int DBDataFile::closeFile()
{
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
}

void DBDataFile::printFileDescription()
{
    dfdp->print();
}

int DBDataFile::addField(const char* name, int type, bool nullable)
{
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
    default:
        DBLogLine("Succeeded in adding field.");
    }
    return re;
}

int DBDataFile::insertRecordToPage(int page, std::vector<void*>& processed)
{
    DBDataPage* dp = (DBDataPage*)(pages[page]);
    return dp->insert(processed);
}

int DBDataFile::insertRecord(std::map<std::string, void*>& fields)
{
    int n = ri->getFieldCount();
    std::vector<void*> processed;
    std::map<std::string, int> errors;
    std::vector<bool> included;
    included.assign(n, false);
    processed.assign(n, NULL);
    errors.clear();
    std::map<std::string, void*>::iterator iter;
    for(iter = fields.begin(); iter != fields.end(); iter++)
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
    if(errors.empty())
    {
        int fadp = findFirstAvailableDataPage();
        std::cout << "First available found: " << fadp << std::endl;
        int re = insertRecordToPage(fadp, processed);
        switch(re)
        {
        case ERROR:
            return ERROR;
        case DATA_PAGE_FULL:
            setAvailableOfDataPage(fadp, false);
        default:
            DBLogLine("Succeeded in inserting record.");
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
}

int DBDataFile::setPrimaryKey(const char* name)
{
    int re = dfdp->setPrimaryKey(name);
    switch(re)
    {
    case FIELD_IS_ALREADY_PRIMARY_KEY:
        DBPrintLine("This field is already the primary key.");
        break;
    case FIELD_NOT_EXIST:
        DBPrintLine("This table does not contain field " + std::string(name));
        break;
    default:
        DBLogLine("Succeeded in altering primary key.");
    }
    return re;
}

int DBDataFile::openFile(const char* name)
{
    if(fm->openFile(name, fileID) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    ri = new DBRecordInfo();
    dfdp = new DBDataFileDescriptionPage(cache, index, 0, MODE_PARSE, ri);
}


