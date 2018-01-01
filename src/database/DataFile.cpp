#include"DataFile.h"

const std::string DataFile::TAG = "DataFile";

DataFile::DataFile(): BaseFile()
{
    fm = FileIOModel::getInstance();
    open = false;
    ri = new RecordInfo();
}

DataFile::~DataFile()
{
	delete ri;
}

RecordInfo* DataFile::getRecordInfo()
{
    return ri;
}

void* printLoop(void* df)
{
    DataFile* f = (DataFile*)df;
    while(true)
    {
        pthread_mutex_lock(&f->search_result_lock);
        if(f->search_result.empty())
        {
            pthread_mutex_unlock(&f->search_result_lock);
            usleep(20);
            continue;
        }
        int rid = f->search_result.front();
        f->search_result.pop();
        pthread_mutex_unlock(&f->search_result_lock);
        if(rid <= 0)
        {
            break;
        }
        f->print(rid);
    }
    return NULL;
}

void DataFile::print(int rid)
{
    std::vector<void*> row;
    ((DataPage*)openPage(rid / PAGE_SIZE))->fetchFields(row, *cur_selected, rid % PAGE_SIZE);
    Table::printRow(row, ri);
}

void* validateLoop(void* df)
{
    DataFile* f = (DataFile*)df;
    while(true)
    {
        pthread_mutex_lock(&f->candidate_lock);
        if(f->candidate_pids.empty())
        {
            pthread_mutex_unlock(&f->candidate_lock);
            usleep(20);
            continue;
        }
        int rid = f->candidate_pids.front();
        f->candidate_pids.pop();
        pthread_mutex_unlock(&f->candidate_lock);
        if(rid <= 0)
        {
            pthread_mutex_lock(&f->search_result_lock);
            f->search_result.push(0);
            pthread_mutex_unlock(&f->search_result_lock);
            break;
        }
        if(f->validate(rid))
        {
            pthread_mutex_lock(&f->search_result_lock);
            f->search_result.push(rid);
            pthread_mutex_unlock(&f->search_result_lock);
        }
    }
    return NULL;
}

bool DataFile::validate(int rid)
{
    return ((DataPage*)openPage(rid / PAGE_SIZE))->validate(*cur_search_info, rid % PAGE_SIZE);
}

void DataFile::terminateSelect(pthread_t* p, pthread_t* v)
{
    pthread_mutex_lock(&candidate_lock);
    candidate_pids.push(0);
    pthread_mutex_unlock(&candidate_lock);
    pthread_join(*v, NULL);
    pthread_join(*p, NULL);
    delete p;
    delete v;
}

void DataFile::select(SearchInfo& si, std::vector<bool>& selected)
{
    Table::printHeader(selected, ri);
    PrimKey::ri = ri;
    cur_search_info = &si;
    cur_selected = &selected;
    pthread_t* printingThread = new pthread_t;
    pthread_t* validatingThread = new pthread_t;
    pthread_create(validatingThread, NULL, validateLoop, this);
    pthread_create(printingThread, NULL, printLoop, this);
    // prim with A
    std::vector<int>& primKeys = ri->getPrimaryKeys();
    bool primA = true;
    for(int i = 0; i < primKeys.size(); i++)
    {
        if(si.nulls.count(i) && si.nulls[i])
        {
            terminateSelect(printingThread, validatingThread);
            return;
        }
        if(!si.values[0].count(i))
        {
            primA = false;
            break;
        }
    }
    if(primA)
    {
        char tmp[ri->getPrimKeyLen()];
        char* cur = tmp;
        for(int i = 0; i < primKeys.size(); i++)
        {
            copyData((char*)si.values[0][primKeys[i]], cur, ri->length(primKeys[i]));
            cur += ri->length(primKeys[i]);
        }
        int rid = tree->search(*(PrimKey*)tmp);
        pthread_mutex_lock(&candidate_lock);
        candidate_pids.push(rid);
        pthread_mutex_unlock(&candidate_lock);
        terminateSelect(printingThread, validatingThread);
        return;
    }
    // unique A
    for(std::map<int, bool>::iterator iter = uniqueIndex.begin(); iter != uniqueIndex.end(); iter++)
    {
        if(iter->second)
        {
            int idx = iter->first;
            if(si.nulls.count(idx) && si.nulls[idx])
            {
                terminateSelect(printingThread, validatingThread);
                return;
            }
            if(si.values[0].count(idx))
            {
                int rid;
                switch(ri->type(idx))
                {
                case Type::INT:
                    rid = ((IndexFile<IntType>*)indexes[idx])->directSearch(*(IntType*)si.values[0][idx]);
                    break;
                case Type::FLOAT:
                    rid = ((IndexFile<FloatType>*)indexes[idx])->directSearch(*(FloatType*)si.values[0][idx]);
                    break;
                case Type::DATE:
                    rid = ((IndexFile<DateType>*)indexes[idx])->directSearch(*(DateType*)si.values[0][idx]);
                    break;
                case Type::VARCHAR:
                    rid = ((IndexFile<VarcharType>*)indexes[idx])->directSearch(*(VarcharType*)si.values[0][idx]);
                    break;
                default:
                    assert(false);
                }
                pthread_mutex_lock(&candidate_lock);
                candidate_pids.push(rid);
                pthread_mutex_unlock(&candidate_lock);
                terminateSelect(printingThread, validatingThread);
                return;
            }
        }
    }
    // non unique A
    for(std::map<int, bool>::iterator iter = uniqueIndex.begin(); iter != uniqueIndex.end(); iter++)
    {
        if(!iter->second)
        {
            int idx = iter->first;
            if(si.nulls.count(idx) && si.nulls[idx])
            {
                terminateSelect(printingThread, validatingThread);
                return;
            }
            if(si.values[0].count(idx))
            {
                switch(ri->type(idx))
                {
                case Type::INT:
                    ((IndexFile<IntType>*)indexes[idx])->getAllValuesOfKey(*(IntType*)si.values[0][idx], candidate_pids, &candidate_lock);
                    break;
                case Type::FLOAT:
                    ((IndexFile<FloatType>*)indexes[idx])->getAllValuesOfKey(*(FloatType*)si.values[0][idx], candidate_pids, &candidate_lock);
                    break;
                case Type::DATE:
                    ((IndexFile<DateType>*)indexes[idx])->getAllValuesOfKey(*(DateType*)si.values[0][idx], candidate_pids, &candidate_lock);
                    break;
                case Type::VARCHAR:
                    ((IndexFile<VarcharType>*)indexes[idx])->getAllValuesOfKey(*(VarcharType*)si.values[0][idx], candidate_pids, &candidate_lock);
                    break;
                default:
                    assert(false);
                }
                terminateSelect(printingThread, validatingThread);
                return;
            }
        }
    }
    std::map<int, std::vector<int> > ranges;
    // -1 for primary key.
    ranges[-1] = std::vector<int>();
    ranges[-1].push_back(-1);
    ranges[-1].push_back(INT_MAX - 1);
    for(int i = 0; i < ri->getFieldCount(); i++)
    {
        ranges[i] = std::vector<int>();
        ranges[i].push_back(-1);
        ranges[i].push_back(INT_MAX - 1);
    }
    //prim B
    if(ri->getPrimKeyCnt() == 1)
    {
        int primKey = ri->getPrimaryKeys()[0];
        int leftBound = -1, rightBound = INT_MAX - 1;
        for(int i = 1; i <= 4; i++)
        {
            if(si.values[i].count(primKey))
            {
                int re = tree->search(*(PrimKey*)si.values[i][primKey], true);
                if(i == 1 || i == 3)
                {
                    rightBound = min(rightBound, re);
                }
                else if(i == 2 || i == 4)
                {
                    leftBound = max(leftBound, re);
                }
            }
        }
        ranges[-1][0] = leftBound;
        ranges[-1][1] = rightBound;
    }
    //unique B
    for(std::map<int, bool>::iterator iter = uniqueIndex.begin(); iter != uniqueIndex.end(); iter++)
    {
        if(iter->second)
        {
            int idx = iter->first;
            int leftBound = -1, rightBound = INT_MAX - 1;
            for(int i = 1; i <= 4; i++)
            {
                if(!si.values[i].count(idx))
                {
                    continue;
                }
                int re;
                switch(ri->type(idx))
                {
                case Type::INT:
                    re = ((IndexFile<IntType>*)indexes[idx])->searchForPos(*(IntType*)si.values[i][idx]);
                    break;
                case Type::FLOAT:
                    re = ((IndexFile<FloatType>*)indexes[idx])->searchForPos(*(FloatType*)si.values[i][idx]);
                    break;
                case Type::DATE:
                    re = ((IndexFile<DateType>*)indexes[idx])->searchForPos(*(DateType*)si.values[i][idx]);
                    break;
                case Type::VARCHAR:
                    re = ((IndexFile<VarcharType>*)indexes[idx])->searchForPos(*(VarcharType*)si.values[i][idx]);
                    break;
                }
                if(i == 1 || i == 3)
                {
                    rightBound = min(rightBound, re);
                }
                else if(i == 2 || i == 4)
                {
                    leftBound = max(leftBound, re);
                }
            }
            ranges[idx][0] = leftBound;
            ranges[idx][1] = rightBound;
        }
    }
    // compare range
    int selectedindex = -2, minDistance = INT_MAX;
    for(int i = -1; i < ri->getFieldCount(); i++)
    {
        if(ranges[i][0] == -1 && ranges[i][1] == INT_MAX - 1)
        {
            continue;
        }
        if(ranges[i][1] - ranges[i][0] < minDistance)
        {
            minDistance = ranges[i][1] - ranges[i][0];
            selectedindex = i;
        }
    }
    if(selectedindex > -2)
    {
        if(selectedindex == -1)
        {
            LeafPage<PrimKey>* cur = (LeafPage<PrimKey>*)openPage(ranges[-1][0] > 0 ? ranges[-1][0] / PAGE_SIZE : dfdp->getFirstLeafPage());
            while(true)
            {
                int cnt = cur->getChildCnt();
                int base = cur->getPageID() * PAGE_SIZE;
                for(int i = 0; i < cnt; i++)
                {
                    if(base + i < ranges[-1][0])
                    {
                        continue;
                    }
                    if(base + i > ranges[-1][1])
                    {
                        break;
                    }
                    pthread_mutex_lock(&candidate_lock);
                    candidate_pids.push(cur->at(i)->value);
                    pthread_mutex_unlock(&candidate_lock);
                }
                if(base + cnt > ranges[-1][1] || cur->getNextSamePage() <= 0)
                {
                    break;
                }
                cur = (LeafPage<PrimKey>*)openPage(cur->getNextSamePage());
            }
            terminateSelect(printingThread, validatingThread);
            return;
        }
        else
        {
            switch(ri->type(selectedindex))
            {
            case Type::INT:
                ((IndexFile<IntType>*)indexes[selectedindex])->fillQueue(ranges[selectedindex][0], ranges[selectedindex][1], candidate_pids, &candidate_lock);
                break;
            case Type::FLOAT:
                ((IndexFile<FloatType>*)indexes[selectedindex])->fillQueue(ranges[selectedindex][0], ranges[selectedindex][1], candidate_pids, &candidate_lock);
                break;
            case Type::DATE:
                ((IndexFile<DateType>*)indexes[selectedindex])->fillQueue(ranges[selectedindex][0], ranges[selectedindex][1], candidate_pids, &candidate_lock);
                break;
            case Type::VARCHAR:
                ((IndexFile<VarcharType>*)indexes[selectedindex])->fillQueue(ranges[selectedindex][0], ranges[selectedindex][1], candidate_pids, &candidate_lock);
                break;
            }
            terminateSelect(printingThread, validatingThread);
            return;
        }
    }
    // non unique B
    for(std::map<int, bool>::iterator iter = uniqueIndex.begin(); iter != uniqueIndex.end(); iter++)
    {
        if(!iter->second)
        {
            int idx = iter->first;
            int leftBound = -1, rightBound = INT_MAX - 1;
            for(int i = 1; i <= 4; i++)
            {
                if(!si.values[i].count(idx))
                {
                    continue;
                }
                int re;
                switch(ri->type(idx))
                {
                case Type::INT:
                    re = ((IndexFile<IntType>*)indexes[idx])->searchForPos(*(IntType*)si.values[i][idx]);
                    break;
                case Type::FLOAT:
                    re = ((IndexFile<FloatType>*)indexes[idx])->searchForPos(*(FloatType*)si.values[i][idx]);
                    break;
                case Type::DATE:
                    re = ((IndexFile<DateType>*)indexes[idx])->searchForPos(*(DateType*)si.values[i][idx]);
                    break;
                case Type::VARCHAR:
                    re = ((IndexFile<VarcharType>*)indexes[idx])->searchForPos(*(VarcharType*)si.values[i][idx]);
                    break;
                }
                if(i == 1 || i == 3)
                {
                    rightBound = min(rightBound, re);
                }
                else if(i == 2 || i == 4)
                {
                    leftBound = max(leftBound, re);
                }
            }
            ranges[idx][0] = leftBound;
            ranges[idx][1] = rightBound;
        }
    }
    selectedindex = -2;
    minDistance = INT_MAX;
    for(int i = 0; i < ri->getFieldCount(); i++)
    {
        if(ranges[i][0] == -1 && ranges[i][1] == INT_MAX - 1)
        {
            continue;
        }
        if(ranges[i][1] - ranges[i][0] < minDistance)
        {
            minDistance = ranges[i][1] - ranges[i][0];
            selectedindex = i;
        }
    }
    if(selectedindex >= 0)
    {
        switch(ri->type(selectedindex))
        {
        case Type::INT:
            ((IndexFile<IntType>*)indexes[selectedindex])->fillQueue(ranges[selectedindex][0], ranges[selectedindex][1], candidate_pids, &candidate_lock);
            break;
        case Type::FLOAT:
            ((IndexFile<FloatType>*)indexes[selectedindex])->fillQueue(ranges[selectedindex][0], ranges[selectedindex][1], candidate_pids, &candidate_lock);
            break;
        case Type::DATE:
            ((IndexFile<DateType>*)indexes[selectedindex])->fillQueue(ranges[selectedindex][0], ranges[selectedindex][1], candidate_pids, &candidate_lock);
            break;
        case Type::VARCHAR:
            ((IndexFile<VarcharType>*)indexes[selectedindex])->fillQueue(ranges[selectedindex][0], ranges[selectedindex][1], candidate_pids, &candidate_lock);
            break;
        }
        terminateSelect(printingThread, validatingThread);
        return;
    }
    // traverse and validate
    int pid = dfdp->getFirstDataPage();
    while(pid > 0)
    {
        int cnt = ((DataPage*)openPage(pid))->recordCnt();
        int base = pid * PAGE_SIZE;
        for(int i = 0; i < cnt; i++)
        {
            pthread_mutex_lock(&candidate_lock);
            candidate_pids.push(base + i);
            pthread_mutex_unlock(&candidate_lock);
        }
        pid = openPage(pid)->getNextSamePage();
    }
    terminateSelect(printingThread, validatingThread);
}

int DataFile::findFirstAvailableDataPage()
{
    assert(open);
    UsagePage* up = (UsagePage*)openPage(1);
    int re;
    while(true)
    {
        if(re = up->findFirstAvailable())
        {
            break;
        }
        int nextUp = up->getNextSamePage();
        if(nextUp > 0)
        {
            up = (UsagePage*)openPage(nextUp);
            continue;
        }
        re = allocateDataPage();
        break;
    }
    return re;
}

void DataFile::printAllRecords()
{
    assert(open);
    std::cout << "All records are listed as below:\n";
    int pid = dfdp->getFirstDataPage();
    while(pid > 0)
    {
        ((DataPage*)openPage(pid))->print();
        pid = openPage(pid)->getNextSamePage();
    }
}

void DataFile::markAsUsable(int n)
{
    int upid = UsagePage::findOccupiedBy(n);
    delete pages[n];
    int index;
    char* cache = fm->getPage(fileID, n, index);
    DataPage* cur = new DataPage(cache, index, n, false, ri);
    pages[n] = cur;
    ((UsagePage*)openPage(upid))->setAvailable(n, true);
    int pid = n;
    while(--pid > 0)
    {
        Page* p = openPage(pid);
        if(p->getPageType() == Type::DATA_PAGE)
        {
            cur->setNextSamePage(p->getNextSamePage());
            if(p->getNextSamePage() > 0)
            {
                openPage(p->getNextSamePage())->setPrevSamePage(n);
            }
            p->setNextSamePage(n);
            cur->setPrevSamePage(pid);
            return;
        }
    }
    dfdp->setFirstDataPage(n);
}

void DataFile::setRootPage(int n)
{
    assert(n > 0);
    dfdp->setRootPage(n);
}

int DataFile::allocateUsagePage()
{
    assert(open);
    int cnt = dfdp->getPageNumber();
    assert(cnt > 0);
    int index;
    char* cache = fm->getPage(fileID, cnt, index);
    UsagePage* up = new UsagePage(cache, index, cnt, false);
    pages[cnt] = up;
//    std::cout << "Allocated new usage page " << cnt << std::endl;
    if(cnt != 1)
    {
        int last = UsagePage::findOccupiedBy(cnt - 1);
        openPage(last)->setNextSamePage(cnt);
        up->setPrevSamePage(last);
    }
    dfdp->incrementPageNumber();
    return cnt;
}

int DataFile::allocateNodePage(bool isLeaf)
{
    assert(open);
    int cnt = dfdp->getPageNumber();
    int upid = UsagePage::findOccupiedBy(cnt);
    if(upid == 0)
    {
        upid = allocateUsagePage();
        cnt = dfdp->getPageNumber();
    }
    UsagePage* up = (UsagePage*)openPage(upid);
    int index;
    char* cache = fm->getPage(fileID, cnt, index);
    if(isLeaf)
    {
        pages[cnt] = new LeafPage<PrimKey>(cache, index, cnt, Type::PRIMARYKEY, ri->getPrimKeyLen(), false);
//        std::cout << "Allocated new leaf page " << cnt << std::endl;
        dfdp->incrementPageNumber();
        if(dfdp->getFirstLeafPage() < 0)
        {
            dfdp->setFirstLeafPage(cnt);
        }
    }
    else
    {
        pages[cnt] = new InternalPage<PrimKey>(cache, index, cnt, Type::PRIMARYKEY, ri->getPrimKeyLen(), false);
//        std::cout << "Allocated new internal page " << cnt << std::endl;
        dfdp->incrementPageNumber();
    }
    up->extendRange(cnt);
    up->setAvailable(cnt, false);
    return cnt;
}

int DataFile::allocateDataPage()
{
    assert(open);
    int cnt = dfdp->getPageNumber();
    int upid = UsagePage::findOccupiedBy(cnt);
    if(upid == 0)
    {
        upid = allocateUsagePage();
        cnt = dfdp->getPageNumber();
    }
    UsagePage* up = (UsagePage*)openPage(upid);
    int index;
    char* cache = fm->getPage(fileID, cnt, index);
    pages[cnt] = new DataPage(cache, index, cnt, false, ri);
//    std::cout << "Allocated new data page " << cnt << std::endl;
    dfdp->incrementPageNumber();
    if(dfdp->getFirstDataPage() < 0)
    {
        dfdp->setFirstDataPage(cnt);
        dfdp->setLastDataPage(cnt);
    }
    else
    {
        openPage(dfdp->getLastDataPage())->setNextSamePage(cnt);
        pages[cnt]->setPrevSamePage(dfdp->getLastDataPage());
        dfdp->setLastDataPage(cnt);
    }
    up->extendRange(cnt);
    up->setAvailable(cnt, true);
    return cnt;
}

char* DataFile::generatePrimKey(std::vector<void*>& data)
{
    //need to assert that they are not null
    char* re = new char[ri->getPrimKeyLen()];
    char* ptr = re;
    for(int i = 0; i < data.size(); i++)
    {
        if(!ri->isPrim(i))
        {
            continue;
        }
        assert(data[i] != NULL);
        copyData((char*)data[i], ptr, ri->length(i));
        ptr += ri->length(i);
    }
    return re;
}

void DataFile::createIndex(std::string fdname, bool unique, int density)
{
    assert(open);
    int idx = ri->index(fdname);
    if(idx == -1)
    {
        throw Exception(TAG, "Field " + fdname + " is never defined in table " + tbname + ".");
    }
    if(indexes.count(idx))
    {
        throw Exception(TAG, "Table " + tbname + " already has an index on " + fdname + ".");
    }
    switch(ri->type(idx))
    {
    case Type::INT:
    {
        IndexFile<IntType>* f = new IndexFile<IntType>();
        f->createFile(dbname, tbname, fdname, ri->type(idx), ri->length(idx), unique, density);
        f->openFile(dbname, tbname, fdname);
        int pid = dfdp->getFirstDataPage();
        while(pid > 0)
        {
            DataPage* dp = (DataPage*)openPage(pid);
            dp->initIterator();
            char* data;
            int ridx;
            data = dp->getNext(idx, ridx);
            while(data != NULL)
            {
                if(!f->insert(*(IntType*)data, pid * PAGE_SIZE + ridx))
                {
                    f->closeFile();
                    fm->dropIdx(dbname, tbname, fdname);
                    throw Exception(TAG, "Error occurred when creating index.");
                }
                data = dp->getNext(idx, ridx);
            }
            pid = dp->getNextSamePage();
        }
//        f->printAllValues();
        indexes[idx] = f;
        break;
    }
    case Type::FLOAT:
    {
        IndexFile<FloatType>* f = new IndexFile<FloatType>();
        f->createFile(dbname, tbname, fdname, ri->type(idx), ri->length(idx), unique, density);
        f->openFile(dbname, tbname, fdname);
        int pid = dfdp->getFirstDataPage();
        while(pid > 0)
        {
            DataPage* dp = (DataPage*)openPage(pid);
            dp->initIterator();
            char* data;
            int ridx;
            data = dp->getNext(idx, ridx);
            while(data != NULL)
            {
                if(!f->insert(*(FloatType*)data, pid * PAGE_SIZE + ridx))
                {
                    f->closeFile();
                    fm->dropIdx(dbname, tbname, fdname);
                    throw Exception(TAG, "Error occurred when creating index.");
                }
                data = dp->getNext(idx, ridx);
            }
            pid = openPage(pid)->getNextSamePage();
        }
        indexes[idx] = f;
        break;
    }
    case Type::DATE:
    {
        IndexFile<DateType>* f = new IndexFile<DateType>();
        f->createFile(dbname, tbname, fdname, ri->type(idx), ri->length(idx), unique, density);
        f->openFile(dbname, tbname, fdname);
        int pid = dfdp->getFirstDataPage();
        while(pid > 0)
        {
            DataPage* dp = (DataPage*)openPage(pid);
            dp->initIterator();
            char* data;
            int ridx;
            data = dp->getNext(idx, ridx);
            while(data != NULL)
            {
                if(!f->insert(*(DateType*)data, pid * PAGE_SIZE + ridx))
                {
                    f->closeFile();
                    fm->dropIdx(dbname, tbname, fdname);
                    throw Exception(TAG, "Error occurred when creating index.");
                }
                data = dp->getNext(idx, ridx);
            }
            pid = openPage(pid)->getNextSamePage();
        }
        indexes[idx] = f;
        break;
    }
    case Type::VARCHAR:
    {
        IndexFile<VarcharType>* f = new IndexFile<VarcharType>();
        f->createFile(dbname, tbname, fdname, ri->type(idx), ri->length(idx), unique, density);
        f->openFile(dbname, tbname, fdname);
        int pid = dfdp->getFirstDataPage();
        while(pid > 0)
        {
            DataPage* dp = (DataPage*)openPage(pid);
            dp->initIterator();
            char* data;
            int ridx;
            data = dp->getNext(idx, ridx);
            while(data != NULL)
            {
                if(!f->insert(*(VarcharType*)data, pid * PAGE_SIZE + ridx))
                {
                    f->closeFile();
                    fm->dropIdx(dbname, tbname, fdname);
                    throw Exception(TAG, "Error occurred when creating index.");
                }
                data = dp->getNext(idx, ridx);
            }
            pid = openPage(pid)->getNextSamePage();
        }
        indexes[idx] = f;
        break;
    }
    }
}

BaseFile* DataFile::getIndexFile(int i)
{
    if(indexes.count(i))
    {
        return indexes[i];
    }
    return NULL;
}

bool DataFile::insert(std::vector<void*>& data)
{
    assert(data[0] == NULL);
    data[0] = new char[Type::typeSize(Type::_ID)];
    write_id((char*)data[0]);
    PrimKey::ri = ri;
    char* primkey = generatePrimKey(data);
    int searchResult = tree->search(*(PrimKey*)primkey);
    if(searchResult > 0)
    {
        return false;
    }
    for(std::map<int, bool>::iterator iter = uniqueIndex.begin(); iter != uniqueIndex.end(); iter++)
    {
        if(iter->second)
        {
            int i = iter->first;
            char* field = (char*)data[i];
            switch(ri->type(i))
            {
            case Type::INT:
                searchResult = ((IndexFile<IntType>*)indexes[i])->directSearch(*(IntType*)field);
                break;
            case Type::FLOAT:
                searchResult = ((IndexFile<FloatType>*)indexes[i])->directSearch(*(FloatType*)field);
                break;
            case Type::DATE:
                searchResult = ((IndexFile<DateType>*)indexes[i])->directSearch(*(DateType*)field);
                break;
            case Type::VARCHAR:
                searchResult = ((IndexFile<VarcharType>*)indexes[i])->directSearch(*(VarcharType*)field);
                break;
            default:
                assert(false);
            }
            if(searchResult > 0)
            {
                return false;
            }
        }
    }
    int dp = findFirstAvailableDataPage();
    DataPage* dpage = (DataPage*)openPage(dp);
    int idx = dpage->insert(data);
    int pid = idx + dpage->getPageID() * PAGE_SIZE;
    if(dpage->isFull())
    {
        int upid = UsagePage::findOccupiedBy(dp);
        ((UsagePage*)openPage(upid))->setAvailable(dp, false);
    }
    tree->insert(*(PrimKey*)primkey, pid);
    for(std::map<int, BaseFile*>::iterator iter = indexes.begin(); iter != indexes.end(); iter++)
    {
        int i = iter->first;
        char* field = (char*)data[i];
        switch(ri->type(i))
        {
        case Type::INT:
            assert(((IndexFile<IntType>*)indexes[i])->insert(*(IntType*)field, pid));
            break;
        case Type::FLOAT:
            assert(((IndexFile<FloatType>*)indexes[i])->insert(*(FloatType*)field, pid));
            break;
        case Type::DATE:
            assert(((IndexFile<DateType>*)indexes[i])->insert(*(DateType*)field, pid));
            break;
        case Type::VARCHAR:
            assert(((IndexFile<VarcharType>*)indexes[i])->insert(*(VarcharType*)field, pid));
            break;
        default:
            assert(false);
        }
    }
    return true;
}

Page* DataFile::openPage(int pid)
{
    assert(open);
    pthread_mutex_lock(&page_lock);
    if(pages.count(pid))
    {
        pthread_mutex_unlock(&page_lock);
        return pages[pid];
    }
    pthread_mutex_unlock(&page_lock);
    assert(pid > 0 && pid < dfdp->getPageNumber());
    int index;
    char* cache = fm->getPage(fileID, pid, index);
    Page* re;
    switch(Page::getPageTypeStatik(cache))
    {
    case Type::DATA_PAGE:
        re = new DataPage(cache, index, pid, true, ri);
        break;
    case Type::INTERNAL_PAGE:
        re = new InternalPage<PrimKey>(cache, index, pid, Type::PRIMARYKEY, ri->getPrimKeyLen(), true);
        break;
    case Type::LEAF_PAGE:
        re = new LeafPage<PrimKey>(cache, index, pid, Type::PRIMARYKEY, ri->getPrimKeyLen(), true);
        break;
    case Type::USAGE_PAGE:
        re = new UsagePage(cache, index, pid, true);
        break;
    default:
        assert(false);
    }
    pthread_mutex_lock(&page_lock);
    pages[pid] = re;
    pthread_mutex_unlock(&page_lock);
    return re;
}

void DataFile::createFile(std::string dbname, std::string tbname)
{
    assert(!open);
    fm->createDataFile(dbname, tbname);
    fm->openDataFile(dbname, tbname, fileID);
    open = true;
    pthread_mutex_init(&candidate_lock, NULL);
    pthread_mutex_init(&search_result_lock, NULL);
    pthread_mutex_init(&page_lock, NULL);
    int index;
    ri->init();
    char* cache = fm->getPage(fileID, 0, index);
    dfdp = new DataFileDescPage(cache, index, 0, false, ri);
    setRootPage(allocateNodePage(true));
    fm->flush(dfdp->getIndex());
    fm->closeFile(fileID);
    open = false;
    delete dfdp;
    pthread_mutex_destroy(&candidate_lock);
    pthread_mutex_destroy(&page_lock);
    pthread_mutex_destroy(&search_result_lock);
}

void DataFile::closeFile()
{
    assert(open);
    fm->flush(dfdp->getIndex());
//    printAllRecords();
    for(std::map<int, BaseFile*>::iterator iter = indexes.begin(); iter != indexes.end(); iter++)
    {

        (iter->second)->closeFile();
        delete iter->second;
    }
    for(std::map<int, Page*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
    {
        fm->flush(iter->second->getIndex());
        delete iter->second;
    }
    indexes.clear();
    uniqueIndex.clear();
    pages.clear();
    open = false;
    fm->closeFile(fileID);
    delete dfdp;
    delete tree;
    pthread_mutex_destroy(&candidate_lock);
    pthread_mutex_destroy(&page_lock);
    pthread_mutex_destroy(&search_result_lock);
}

void DataFile::printFileDesc()
{
    assert(open);
    dfdp->print();
}

void DataFile::addFields(std::vector<std::string>& name, std::vector<int>& type, std::vector<int>& nullable,
		std::vector<int>& extra, std::vector<std::string>& foreign, std::vector<bool>& isPrim)
{
    assert(open);
    assert(name.size() == type.size() &&
       type.size() == nullable.size() &&
       nullable.size() == extra.size() &&
	   extra.size() == foreign.size() &&
       foreign.size() == isPrim.size());
    for(int i = 0; i < name.size(); i++)
    {
        int re = ri->addField(name[i], type[i], nullable[i], extra[i], foreign[i], isPrim[i]);
        switch(re)
        {
        case RecordInfo::EMPTY_FIELD_NAME:
            throw Exception(TAG, "Field name cannot be empty.");
        case RecordInfo::FIELD_ALREADY_EXIST:
            throw Exception(TAG, "Field " + name[i] + " already exists.");
        case RecordInfo::EXCEED_PAGE_LIMIT:
            throw Exception(TAG, "You cannot add any more fields to this table.");
        }
    }
    dfdp->writeFields();
}

void DataFile::openFile(std::string dbname, std::string tbname)
{
    assert(!open);
    fm->openDataFile(dbname, tbname, fileID);
    this->tbname = tbname;
    this->dbname = dbname;
    open = true;
    int index;
    ri->init();
    char* cache = fm->getPage(fileID, 0, index);
    dfdp = new DataFileDescPage(cache, index, 0, true, ri);
    tree = new IndexTree<PrimKey>(this, dfdp->getRootPage(), ri->getPrimKeyLen());
    for(int i = 1; i < ri->getFieldCount(); i++)
    {
        if(fm->checkIdxFileExist(dbname, tbname, ri->name(i)))
        {
            BaseFile* bf;
            switch(ri->type(i))
            {
            case Type::INT:
                bf = new IndexFile<IntType>();
                ((IndexFile<IntType>*)bf)->openFile(dbname, tbname, ri->name(i));
                uniqueIndex[i] = ((IndexFile<IntType>*)bf)->isUnique();
                break;
            case Type::FLOAT:
                bf = new IndexFile<FloatType>();
                ((IndexFile<FloatType>*)bf)->openFile(dbname, tbname, ri->name(i));
                uniqueIndex[i] = ((IndexFile<FloatType>*)bf)->isUnique();
                break;
            case Type::DATE:
                bf = new IndexFile<DateType>();
                ((IndexFile<DateType>*)bf)->openFile(dbname, tbname, ri->name(i));
                uniqueIndex[i] = ((IndexFile<DateType>*)bf)->isUnique();
                break;
            case Type::VARCHAR:
                bf = new IndexFile<VarcharType>();
                ((IndexFile<VarcharType>*)bf)->openFile(dbname, tbname, ri->name(i));
                uniqueIndex[i] = ((IndexFile<VarcharType>*)bf)->isUnique();
                break;
            default:
                assert(false);
            }
            indexes[i] = bf;
        }
    }
    pthread_mutex_init(&candidate_lock, NULL);
    pthread_mutex_init(&search_result_lock, NULL);
    pthread_mutex_init(&page_lock, NULL);
}

void DataFile::test()
{
//	try{FileIOModel::getInstance()->dropTable("test", "test");} catch(Exception){}
//	DataFile df;
//	df.createFile("test", "test");
//	df.openFile("test", "test");
//	std::vector<std::string> name;
//	name.push_back("_id");
//	name.push_back("test");
//	std::vector<int> type;
//	type.push_back(0);
//	type.push_back(1);
//	std::vector<int> nullable;
//	nullable.push_back(0);
//	nullable.push_back(0);
//	std::vector<int> extra;
//	extra.push_back(0);
//	extra.push_back(0);
//	std::vector<std::string> foreign;
//	foreign.push_back("");
//	foreign.push_back("");
//	df.addFields(name, type, nullable, extra, foreign, 2);
//	df.closeFile();
//	df.openFile("test", "test");
//	df.printFileDesc();
//	PrimKey::ri = df.ri;
//	std::vector<void*> data;
//	for(int i = 0; i < 1000000; i++)
//    {
//        data.clear();
//        char tmp[16];
//        write_id(tmp);
//        data.push_back(tmp);
//        data.push_back(&i);
//        assert(df.insert(data));
//        std::cout << "inserted " << i <<"\n";
//    }
//	df.closeFile();
	//FileIOModel::getInstance()->dropTable("test", "test");
}
