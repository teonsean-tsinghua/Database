#include"DBDataFile.h"

SelectResult::SelectResult()
{

}

void SelectResult::filterByValue(std::map<int, std::vector<void*> >& info, int op, DBRecordInfo* ri)
{
    std::list<std::vector<void*> >::iterator it;
    for(it = results.begin(); it != results.end(); )
    {
        std::vector<void*>& data = *it;
        std::map<int, std::vector<void*> >::iterator iter;
        bool flag = true;
        for(iter = info.begin(); iter != info.end(); iter++)
        {
            int idx = iter->first;
            if(data[idx] == NULL)
            {
                flag = false;
                break;
            }
            std::vector<void*>& vecs = iter->second;
            for(int i = 0; i < vecs.size(); i++)
            {
                switch(op)
                {
                case 0:
                    flag = Equal(data[idx], vecs[i], ri->type(idx), ri->length(idx));
                    break;
                case 1:
                    flag = !Equal(data[idx], vecs[i], ri->type(idx), ri->length(idx));
                    break;
                case 2:
                    flag = smallerOrEqual(data[idx], vecs[i], ri->type(idx), ri->length(idx));
                    break;
                case 3:
                    flag = largerOrEqual(data[idx], vecs[i], ri->type(idx), ri->length(idx));
                    break;
                case 4:
                    flag = smaller(data[idx], vecs[i], ri->type(idx), ri->length(idx));
                    break;
                case 5:
                    flag = larger(data[idx], vecs[i], ri->type(idx), ri->length(idx));
                    break;
                }
                if(!flag)
                {
                    break;
                }
            }
            if(!flag)
            {
                break;
            }
        }
        if(!flag)
        {
            it = results.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void SelectResult::filterByFields(std::map<int, std::vector<int> >& info, int op, DBRecordInfo* ri)
{
    std::list<std::vector<void*> >::iterator it;
    for(it = results.begin(); it != results.end(); )
    {
        std::vector<void*>& data = *it;
        std::map<int, std::vector<int> >::iterator iter;
        bool flag = true;
        for(iter = info.begin(); iter != info.end(); iter++)
        {
            std::vector<int>& vecs = iter->second;
            for(int i = 0; i < vecs.size(); i++)
            {
                int lidx = iter->first;
                int ridx = vecs[i];
                bool lnull = (data[lidx] == NULL);
                bool rnull = (data[ridx] == NULL);
                if(lnull && rnull)
                {
                    if(op != 0)
                    {
                        flag = false;
                        break;
                    }
                }
                if((lnull && !rnull) || (!lnull && rnull))
                {
                    if(op != 1)
                    {
                        flag = false;
                        break;
                    }
                }
                switch(op)
                {
                case 0:
                    flag = Equal(data[lidx], data[ridx], ri->type(lidx), ri->length(lidx));
                    break;
                case 1:
                    flag = !Equal(data[lidx], data[ridx], ri->type(lidx), ri->length(lidx));
                    break;
                case 2:
                    flag = smallerOrEqual(data[lidx], data[ridx], ri->type(lidx), ri->length(lidx));
                    break;
                case 3:
                    flag = largerOrEqual(data[lidx], data[ridx], ri->type(lidx), ri->length(lidx));
                    break;
                case 4:
                    flag = smaller(data[lidx], data[ridx], ri->type(lidx), ri->length(lidx));
                    break;
                case 5:
                    flag = larger(data[lidx], data[ridx], ri->type(lidx), ri->length(lidx));
                    break;
                }
                if(!flag)
                {
                    break;
                }
            }
            if(!flag)
            {
                break;
            }
        }
        if(!flag)
        {
            it = results.erase(it);
        }
        else
        {
            it++;
        }
    }
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

int DBDataFile::remove(SearchInfo& si)
{
    assert(open);
    DBDataPage* dp = openDataPage(dfdp->getFirstDataPage());
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

void DBDataFile::update(std::map<std::string, void*>& key_value, std::map<std::string, void*>& update_value)
{
//    assert(open);
//    std::map<int, void*> processed;
//    std::vector<std::string> errors;
//    processKeyValue(key_value, processed, errors);
//    if(errors.empty())
//    {
//        std::map<int, void*> processed2;
//        std::vector<std::string> errors2;
//        processKeyValue(update_value, processed2, errors2);
//        if(errors2.empty())
//        {
//            DBDataPage* dp = openDataPage(dfdp->getFirstDataPage());
//            while(true)
//            {
//                if(dp == NULL)
//                {
//                    break;
//                }
//                dp->update(processed, processed2);
//                dp = openDataPage(dp->getNextSameType());
//            }
//        }
//        else
//        {
//            for(int i = 0; i < errors.size(); i++)
//            {
//                DBPrint::printLine("This table does not contain field " + errors[i]);
//            }
//        }
//    }
//    else
//    {
//        for(int i = 0; i < errors.size(); i++)
//        {
//            DBPrint::printLine("This table does not contain field " + errors[i]);
//        }
//    }
}

void DBDataFile::select(SearchInfo& si, SelectResult& sr)
{
    assert(open);
//    int cnt = si.nulls.size();
//    for(int i = 0; i < 6; i++)
//    {
//        for(std::map<int, std::vector<void*> >::iterator iter = si.values[i].begin(); iter != si.values[i].end(); iter++)
//        {
//            cnt += (iter->second).size();
//        }
//        for(std::map<int, std::vector<int> >::iterator iter = si.fields[i].begin(); iter != si.fields[i].end(); iter++)
//        {
//            cnt += (iter->second).size();
//        }
//    }
//    DBPrint::print(cnt).printLine(" conditions.");
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
        if(sr.results.empty())
        {
            return;
        }
    }
    for(int i = 0; i < 6; i++)
    {
        if(si.values[i].size() > 0)
        {
            if(sr.results.empty())
            {
                DBDataPage* dp = openDataPage(dfdp->getFirstDataPage());
                while(true)
                {
                    if(dp == NULL)
                    {
                        break;
                    }
                    dp->filterByValue(si.values[i], sr.results, i);
                    dp = openDataPage(dp->getNextSameType());
                }
                if(sr.results.empty())
                {
                    return;
                }
            }
            else
            {
                DBDataPage* dp = openDataPage(dfdp->getFirstDataPage());
                while(true)
                {
                    if(dp == NULL)
                    {
                        break;
                    }
                    sr.filterByValue(si.values[i], i, ri);
                    dp = openDataPage(dp->getNextSameType());
                }
                if(sr.results.empty())
                {
                    return;
                }
            }
        }
    }
    for(int i = 0; i < 6; i++)
    {
        if(si.fields[i].size() > 0)
        {
            DBPrint::printLine("checking fields");
            if(sr.results.empty())
            {
                DBDataPage* dp = openDataPage(dfdp->getFirstDataPage());
                while(true)
                {
                    if(dp == NULL)
                    {
                        break;
                    }
                    dp->filterByFields(si.fields[i], sr.results, i);
                    dp = openDataPage(dp->getNextSameType());
                }
                if(sr.results.empty())
                {
                    return;
                }
            }
            else
            {
                DBDataPage* dp = openDataPage(dfdp->getFirstDataPage());
                while(true)
                {
                    if(dp == NULL)
                    {
                        break;
                    }
                    sr.filterByFields(si.fields[i], i, ri);
                    dp = openDataPage(dp->getNextSameType());
                }
                if(sr.results.empty())
                {
                    return;
                }
            }
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
