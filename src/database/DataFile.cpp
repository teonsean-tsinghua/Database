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

//void DataFile::printAllRecords()
//{
//    assert(open);
//    DataPage* dp = openDataPage(dfdp->getFirstDataPage());
//    while(true)
//    {
//        if(dp == NULL)
//        {
//            return;
//        }
//        dp->print();
//        dp->printAllRecords();
//        Print::printLine("====================");
//        dp = openDataPage(dp->getNextSameType());
//    }
//}
//
//int DataFile::findFirstAvailableDataPage()
//{
//    assert(open);
//    UsagePage* up = openUsagePage(dfdp->getFirstUsagePage());
//    while(true)
//    {
//        if(up == NULL)
//        {
//            return allocateNewDataPage();
//        }
//        int re = up->findFirstAvailable();
//        if(re > 0 && re < dfdp->getPageNumber())
//        {
//            return re;
//        }
//        up = openUsagePage(up->getNextSameType());
//    }
//}

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
    std::cout << "Allocated new usage page " << cnt << std::endl;
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
        std::cout << "Allocated new internal page " << cnt << std::endl;
        dfdp->incrementPageNumber();
    }
    up->extendRange(cnt);
    up->setAvailable(cnt, true);
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
    std::cout << "Allocated new data page " << cnt << std::endl;
    dfdp->incrementPageNumber();
    if(dfdp->getFirstDataPage() < 0)
    {
        dfdp->setFirstDataPage(-1);
        dfdp->setLastDataPage(-1);
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

Page* DataFile::openPage(int pid)
{
    assert(open);
    if(pages.count(pid))
    {
        return pages[pid];
    }
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
    }
    pages[pid] = re;
    return re;
}

void DataFile::createFile(std::string dbname, std::string tbname)
{
    assert(!open);
    fm->createDataFile(dbname, tbname);
    fm->openDataFile(dbname, tbname, fileID);
    open = true;
    int index;
    ri->init();
    char* cache = fm->getPage(fileID, 0, index);
    dfdp = new DataFileDescPage(cache, index, 0, false, ri);
    setRootPage(allocateNodePage(true));
    fm->flush(dfdp->getIndex());
    fm->closeFile(fileID);
    open = false;
    delete dfdp;
}

void DataFile::closeFile()
{
    assert(open);
    fm->flush(dfdp->getIndex());
    for(std::map<int, Page*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
    {
        fm->flush(iter->second->getIndex());
        delete iter->second;
    }
    pages.clear();
    open = false;
    fm->closeFile(fileID);
    delete dfdp;
    delete tree;
}

void DataFile::printFileDesc()
{
    assert(open);
    dfdp->print();
}

void DataFile::addFields(std::vector<std::string>& name, std::vector<int>& type, std::vector<int>& nullable,
		std::vector<int>& extra, std::vector<std::string>& foreign, int primCnt)
{
    assert(open);
    assert(primCnt >= 1);
    assert(name.size() == type.size() &&
       type.size() == nullable.size() &&
       nullable.size() == extra.size() &&
	   extra.size() == foreign.size());
    for(int i = 0; i < name.size(); i++)
    {
        int re = ri->addField(name[i], type[i], nullable[i], extra[i], foreign[i]);
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
    dfdp->setPrimaryKeyCount(primCnt);
}

void DataFile::openFile(std::string dbname, std::string tbname)
{
    assert(!open);
    fm->openDataFile(dbname, tbname, fileID);
    open = true;
    int index;
    ri->init();
    char* cache = fm->getPage(fileID, 0, index);
    dfdp = new DataFileDescPage(cache, index, 0, true, ri);
    tree = new IndexTree<PrimKey>(this, dfdp->getRootPage(), ri->getPrimKeyLen());
}

void DataFile::test()
{
	try{FileIOModel::getInstance()->dropTable("test", "test");} catch(Exception){}
	DataFile df;
	df.createFile("test", "test");
	df.openFile("test", "test");
	std::vector<std::string> name;
	name.push_back("test");
	std::vector<int> type;
	type.push_back(1);
	std::vector<int> nullable;
	nullable.push_back(0);
	std::vector<int> extra;
	extra.push_back(0);
	std::vector<std::string> foreign;
	foreign.push_back("");
	df.addFields(name, type, nullable, extra, foreign, 1);
	df.closeFile();
	df.openFile("test", "test");
	df.printFileDesc();
	PrimKey::ri = df.ri;
	std::set<int> numbers;
	for(int i = 0; i < 1000000; i++)
    {
        numbers.insert(rand() * rand());
    }
    std::cout << "Numbers generated.\n";
    for(std::set<int>::iterator iter = numbers.begin(); iter != numbers.end(); iter++)
    {
        int n = *iter;
        df.tree->insert(*(PrimKey*)&n, n);
    }
    std::cout << "Finished inserting.\n";
    for(std::set<int>::iterator iter = numbers.begin(); iter != numbers.end(); iter++)
    {
        int n = *iter;
        assert(df.tree->search(*(PrimKey*)&n) == n);
    }
    std::cout << "Finished searching.\n";
    for(std::set<int>::iterator iter = numbers.begin(); iter != numbers.end(); iter++)
    {
        int n = *iter;
        assert(df.tree->remove(*(PrimKey*)&n));
    }
    std::cout << "Finished removing.\n";
//	for(int i = 1000000 - 1; i >= 0; i--)
//    {
//        key[i] = new char[4];
//        writeInt(key[i], i);
//        df.tree->insert(*(PrimKey*)key[i], i);
////        std::cout << i << " inserted.\n";
//    }
//    for(int i = 1000000 - 1; i >= 0; i--)
//    {
////        std::cout << "Removing " << i << "\n";
//        assert(df.tree->remove(*(PrimKey*)key[i]));
////        int p = df.dfdp->getFirstLeafPage();
////        int head, tail = -1;
////        do
////        {
////            NodePage<PrimKey>* cur = (NodePage<PrimKey>*)df.openPage(p);
////            head = cur->at(0)->value;
////            if(p != df.dfdp->getFirstLeafPage())
////            assert(head == tail + 1);
////            tail = cur->at(cur->getChildCnt() - 1)->value;
////            p = cur->getNextSamePage();
////        }while(p > 0);
//    }
	df.closeFile();
	FileIOModel::getInstance()->dropTable("test", "test");
}
