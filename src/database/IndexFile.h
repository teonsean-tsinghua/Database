#ifndef INDEXFILE_H_INCLUDED
#define INDEXFILE_H_INCLUDED

#include "IndexFileDescPage.h"
#include "NodePage.h"
#include "UsagePage.h"
#include "FileIOModel.h"
#include "BucketPage.h"
#include "IndexTree.h"

template<typename T>
class IndexFile: public BaseFile
{
private:
    int keyLength;
    int keyType;
    IndexFileDescPage* ifdp;
    IndexTree<T>* tree;
    std::map<int, Page*> pages;
    bool open;

    int findFirstAvailableBucketPage();

    int allocateUsagePage();

    int allocateNodePage(bool isLeaf);

    int allocateBucketPage();

    void markAsUsable(int n);

    void setRootPage(int n);

    Page* openPage(int pid);


public:
	IndexFile();

	void createFile(std::string dbname, std::string tbname, std::string fdname, int keyType, int keyLength, bool unique, int density = 1);

	void openFile(std::string dbname, std::string tbname, std::string fdname);

	void closeFile();

    void printFileDesc();

    bool insert(T& key, int value);

    bool remove(T& key, int value = 0);

    void fillQueue(int begin, int end, std::queue<int>& q, pthread_mutex_t* lock);

    int directSearch(T& key);

    int searchForPos(T& key);

    bool update(T& key, int value, int old_value = 0);

    void printValuesOfKey(T& key);

    void getAllValuesOfKey(T& key, std::queue<int>& result, pthread_mutex_t* mutex);

    void printAllValues();

    bool isUnique();

    static void test();

};

template<typename T>
IndexFile<T>::IndexFile()
{
	fm = FileIOModel::getInstance();
    fileID = -1;
    open = false;
}

template<typename T>
void IndexFile<T>::fillQueue(int begin, int end, std::queue<int>& q, pthread_mutex_t* lock)
{
    assert(open);
    if(ifdp->getUnique())
    {
        LeafPage<T>* cur = (LeafPage<T>*)openPage(begin > 0 ? begin / PAGE_SIZE : ifdp->getFirstLeafPage());
        while(true)
        {
            int cnt = cur->getChildCnt();
            int base = cur->getPageID() * PAGE_SIZE;
            for(int i = 0; i < cnt; i++)
            {
                if(base + i < begin)
                {
                    continue;
                }
                if(base + i > end)
                {
                    break;
                }
                pthread_mutex_lock(lock);
                q.push(cur->at(i)->value);
                pthread_mutex_unlock(lock);
            }
            if(base + cnt - 1 > end || cur->getNextSamePage() <= 0)
            {
                break;
            }
            cur = (LeafPage<T>*)openPage(cur->getNextSamePage());
        }
    }
    else
    {
        LeafPage<T>* cur = (LeafPage<T>*)openPage(begin > 0 ? begin / PAGE_SIZE : ifdp->getFirstLeafPage());
        while(true)
        {
            int cnt = cur->getChildCnt();
            int base = cur->getPageID() * PAGE_SIZE;
            for(int i = 0; i < cnt; i++)
            {
                if(base + i < begin)
                {
                    continue;
                }
                if(base + i > end)
                {
                    break;
                }
                int bid = cur->at(i)->value;
                Bucket* target = ((BucketPage*)openPage(bid / PAGE_SIZE))->at(bid % PAGE_SIZE);
                while(true)
                {
                    Bucket::fillQueue(target, ifdp->getDensity(), q, lock);
                    if(target->next <= 0)
                    {
                        break;
                    }
                    target = ((BucketPage*)openPage(target->next / PAGE_SIZE))->at(target->next % PAGE_SIZE);
                }
            }
            if(base + cnt - 1 > end || cur->getNextSamePage() <= 0)
            {
                break;
            }
            cur = (LeafPage<T>*)openPage(cur->getNextSamePage());
        }
    }
}

template<typename T>
void IndexFile<T>::getAllValuesOfKey(T& key, std::queue<int>& result, pthread_mutex_t* mutex)
{
    assert(open);
    assert(!ifdp->getUnique());
    int bid = tree->search(key);
    if(bid <= 0)
    {
        std::cout << 0 << std::endl;
        return;
    }
    Bucket* target = ((BucketPage*)openPage(bid / PAGE_SIZE))->at(bid % PAGE_SIZE);
    while(true)
    {
        Bucket::fillQueue(target, ifdp->getDensity(), result, mutex);
        if(target->next <= 0)
        {
            break;
        }
        target = ((BucketPage*)openPage(target->next / PAGE_SIZE))->at(target->next % PAGE_SIZE);
    }
}

template<typename T>
int IndexFile<T>::directSearch(T& key)
{
    return tree->search(key);
}

template<typename T>
int IndexFile<T>::searchForPos(T& key)
{
    return tree->searchForIdx(key);
}

template<typename T>
bool IndexFile<T>::isUnique()
{
    return ifdp->getUnique();
}

template<typename T>
void IndexFile<T>::printAllValues()
{
    int pid = ifdp->getFirstLeafPage();
    while(pid > 0)
    {
        NodePage<T>* np = (NodePage<T>*)openPage(pid);
        int cnt = np->getChildCnt();
        for(int i = 0; i < cnt; i++)
        {
            printValuesOfKey(np->at(i)->key);
        }
        pid = np->getNextSamePage();
    }
}

template<typename T>
void IndexFile<T>::printValuesOfKey(T& key)
{
    assert(open);
    std::cout << "Values of key " << key << ":\n";
    if(ifdp->getUnique())
    {
        std::cout << tree->search(key) << std::endl;
    }
    int bid = tree->search(key);
    if(bid <= 0)
    {
        std::cout << 0 << std::endl;
        return;
    }
    Bucket* target = ((BucketPage*)openPage(bid / PAGE_SIZE))->at(bid % PAGE_SIZE);
    while(true)
    {
        Bucket::print(target, ifdp->getDensity());
        if(target->next <= 0)
        {
            break;
        }
        target = ((BucketPage*)openPage(target->next / PAGE_SIZE))->at(target->next % PAGE_SIZE);
    }
    std::cout << "===================================================\n";
}

template<typename T>
bool IndexFile<T>::update(T& key, int value, int old_value)
{
    assert(open);
    if(ifdp->getUnique())
    {
        return tree->update(key, value);
    }
    assert(old_value > 0);
    int bucket_id = tree->search(key);
    if(bucket_id <= 0)
    {
        return false;
    }
    Bucket* target = ((BucketPage*)openPage(bucket_id / PAGE_SIZE))->at(bucket_id % PAGE_SIZE);
    bool re;
    while(true)
    {
        int idx;
        if((idx = Bucket::search(target, old_value, ifdp->getDensity())) >= 0)
        {
            re = true;
            target->pids[idx] = value;
            break;
        }
        if(target->next <= 0)
        {
            re = false;
            break;
        }
        target = ((BucketPage*)openPage(target->next / PAGE_SIZE))->at(target->next % PAGE_SIZE);
    }
    return re;
}

template<typename T>
bool IndexFile<T>::remove(T& key, int value)
{
    assert(open);
    if(ifdp->getUnique())
    {
        return tree->remove(key);
    }
    assert(value > 0);
    int bucket_id = tree->search(key);
    if(bucket_id <= 0)
    {
        return false;
    }
    Bucket* target = ((BucketPage*)openPage(bucket_id / PAGE_SIZE))->at(bucket_id % PAGE_SIZE);
    int tail_bid = bucket_id;
    Bucket* tail = target, *secondLast = NULL;
    while(tail->next > 0)
    {
        tail_bid = tail->next;
        secondLast = tail;
        tail = ((BucketPage*)openPage(tail_bid / PAGE_SIZE))->at(tail_bid % PAGE_SIZE);
    }
    int tail_index = Bucket::lastIdx(tail, ifdp->getDensity());
    assert(tail_index >= 0);
    bool re;
    while(true)
    {
        int idx;
        if((idx = Bucket::search(target, value, ifdp->getDensity())) >= 0)
        {
            target->pids[idx] = tail->pids[tail_index];
            tail->pids[tail_index] = 0;
            if(Bucket::lastIdx(tail, ifdp->getDensity()) < 0)
            {
                BucketPage* page = (BucketPage*)openPage(tail_bid / PAGE_SIZE);
                page->setAvailable(tail_bid % PAGE_SIZE, true);
                if(secondLast != NULL)
                {
                    secondLast->next = 0;
                    std::cout << secondLast->next << "\n";
                }
                if(bucket_id == tail_bid)
                {
                    tree->remove(key);
                }
                int upid = UsagePage::findOccupiedBy(page->getPageID());
                ((UsagePage*)openPage(upid))->setAvailable(page->getPageID(), true);
            }
            re = true;
            break;
        }
        int next = target->next;
        if(next <= 0)
        {
            re = false;
            break;
        }
        target = ((BucketPage*)openPage(next / PAGE_SIZE))->at(next % PAGE_SIZE);
    }
    return re;
}

template<typename T>
bool IndexFile<T>::insert(T& key, int value)
{
    assert(open);
    if(ifdp->getUnique())
    {
        if(tree->search(key) != 0)
        {
            return false;
        }
        tree->insert(key, value);
        return true;
    }
    int bucket_id = tree->search(key);
    Bucket* target = NULL;
    if(bucket_id <= 0)
    {
        int bucket_pid = findFirstAvailableBucketPage();
        int idx;
        BucketPage* page = ((BucketPage*)openPage(bucket_pid));
        target = page->allocateBucket(idx);
        bucket_id = PAGE_SIZE * bucket_pid + idx;
        tree->insert(key, bucket_id);
        if(page->full())
        {
            int upid = UsagePage::findOccupiedBy(page->getPageID());
            ((UsagePage*)openPage(upid))->setAvailable(page->getPageID(), false);
        }
    }
    else
    {
        target = ((BucketPage*)openPage(bucket_id / PAGE_SIZE))->at(bucket_id % PAGE_SIZE);
    }
    bool re;
    while(true)
    {
        if(Bucket::containForInsert(target, value, ifdp->getDensity(), re))
        {
            re = !re;
            break;
        }
        if(target->next <= 0)
        {
            int pid = findFirstAvailableBucketPage();
            int idx;
            BucketPage* page = ((BucketPage*)openPage(pid));
            Bucket* new_bucket = page->allocateBucket(idx);
            target->next = PAGE_SIZE * pid + idx;
            target = new_bucket;
            if(page->full())
            {
                int upid = UsagePage::findOccupiedBy(page->getPageID());
                ((UsagePage*)openPage(upid))->setAvailable(page->getPageID(), false);
            }
        }
        else
        {
            target = ((BucketPage*)openPage(target->next / PAGE_SIZE))->at(target->next % PAGE_SIZE);
        }
    }
    return re;
}

template<typename T>
void IndexFile<T>::setRootPage(int n)
{
    assert(open);
    ifdp->setRootPage(n);
}

template<typename T>
int IndexFile<T>::findFirstAvailableBucketPage()
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
        re = allocateBucketPage();
        break;
    }
    return re;
}

template<typename T>
void IndexFile<T>::markAsUsable(int n)
{
    assert(open);
    int upid = UsagePage::findOccupiedBy(n);
    delete pages[n];
    int index;
    char* cache = fm->getPage(fileID, n, index);
    BucketPage* cur = new BucketPage(cache, index, n, false, ifdp->getDensity());
    pages[n] = cur;
    ((UsagePage*)openPage(upid))->setAvailable(n, true);
    int pid = n;
    while(--pid > 0)
    {
        Page* p = openPage(pid);
        if(p->getPageType() == Type::BUCKET_PAGE)
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
    ifdp->setFirstBucketPage(n);
}

template<typename T>
int IndexFile<T>::allocateUsagePage()
{
    assert(open);
    int cnt = ifdp->getPageNumber();
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
    ifdp->incrementPageNumber();
    return cnt;
}

template<typename T>
int IndexFile<T>::allocateNodePage(bool isLeaf)
{
    assert(open);
    int cnt = ifdp->getPageNumber();
    int upid = UsagePage::findOccupiedBy(cnt);
    if(upid == 0)
    {
        upid = allocateUsagePage();
        cnt = ifdp->getPageNumber();
    }
    UsagePage* up = (UsagePage*)openPage(upid);
    int index;
    char* cache = fm->getPage(fileID, cnt, index);
    if(isLeaf)
    {
        pages[cnt] = new LeafPage<T>(cache, index, cnt, keyType, keyLength, false);
//        std::cout << "Allocated new leaf page " << cnt << std::endl;
        ifdp->incrementPageNumber();
        if(ifdp->getFirstLeafPage() < 0)
        {
            ifdp->setFirstLeafPage(cnt);
        }
    }
    else
    {
        pages[cnt] = new InternalPage<T>(cache, index, cnt, keyType, keyLength, false);
//        std::cout << "Allocated new internal page " << cnt << std::endl;
        ifdp->incrementPageNumber();
    }
    up->extendRange(cnt);
    up->setAvailable(cnt, false);
    return cnt;
}

template<typename T>
int IndexFile<T>::allocateBucketPage()
{
    assert(open);
    int cnt = ifdp->getPageNumber();
    int upid = UsagePage::findOccupiedBy(cnt);
    if(upid == 0)
    {
        upid = allocateUsagePage();
        cnt = ifdp->getPageNumber();
    }
    UsagePage* up = (UsagePage*)openPage(upid);
    int index;
    char* cache = fm->getPage(fileID, cnt, index);
    pages[cnt] = new BucketPage(cache, index, cnt, false, ifdp->getDensity());
//    std::cout << "Allocated new bucket page " << cnt << std::endl;
    ifdp->incrementPageNumber();
    if(ifdp->getFirstBucketPage() < 0)
    {
        ifdp->setFirstBucketPage(cnt);
        ifdp->setLastBucketPage(cnt);
    }
    else
    {
        openPage(ifdp->getLastBucketPage())->setNextSamePage(cnt);
        pages[cnt]->setPrevSamePage(ifdp->getLastBucketPage());
        ifdp->setLastBucketPage(cnt);
    }
    up->extendRange(cnt);
    up->setAvailable(cnt, true);
    return cnt;
}

template<typename T>
void IndexFile<T>::printFileDesc()
{
    assert(open);
    ifdp->print();
}

template<typename T>
Page* IndexFile<T>::openPage(int pid)
{
    assert(open);
	if(pages.count(pid))
    {
		return pages[pid];
	}
    assert(pid > 0 && pid < ifdp->getPageNumber());
	int index;
	char* cache = fm->getPage(fileID, pid, index);
	int pageType = Page::getPageTypeStatik(cache);
	switch(pageType)
	{
    case Type::INTERNAL_PAGE:
    {
        InternalPage<T>* re = new InternalPage<T>(cache, index, pid, keyType, keyLength, true);
        pages[pid] = re;
        return re;
    }
    case Type::LEAF_PAGE:
    {
        LeafPage<T>* re  = new LeafPage<T>(cache, index, pid, keyType, keyLength, true);
        pages[pid] = re;
        return re;
    }
    case Type::USAGE_PAGE:
    {
        UsagePage* re  = new UsagePage(cache, index, pid, true);
        pages[pid] = re;
        return re;
    }
    case Type::BUCKET_PAGE:
    {
        BucketPage* re  = new BucketPage(cache, index, pid, true, ifdp->getDensity());
        pages[pid] = re;
        return re;
    }
    default:
        assert(false);
    }
    return NULL;
}

template<typename T>
void IndexFile<T>::createFile(std::string dbname, std::string tbname, std::string fdname, int keyType, int keyLength, bool unique, int density)
{
    assert(!open);
    fm->createIndexFile(dbname, tbname, fdname);
    fm->openIdxFile(dbname, tbname, fdname, fileID);
    this->keyType = keyType;
    this->keyLength = keyLength;
    open = true;
    int index;
    char* cache = fm->getPage(fileID, 0, index);
    ifdp = new IndexFileDescPage(cache, index, 0, false, keyType, keyLength);
    ifdp->setUnique(unique);
    ifdp->setDensity(density);
    ifdp->setRootPage(allocateNodePage(true));
    fm->flush(ifdp->getIndex());
    for(std::map<int, Page*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
    {
        fm->flush(iter->second->getIndex());
        delete iter->second;
    }
    pages.clear();
    fm->closeFile(fileID);
    open = false;
    delete ifdp;
}

template<typename T>
void IndexFile<T>::closeFile()
{
    assert(open);
    fm->flush(ifdp->getIndex());
//    printAllValues();
    for(std::map<int, Page*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
    {
        fm->flush(iter->second->getIndex());
        delete iter->second;
    }
    pages.clear();
    open = false;
    fm->closeFile(fileID);
    delete ifdp;
}

template<typename T>
void IndexFile<T>::openFile(std::string dbname, std::string tbname, std::string fdname)
{
    assert(!open);
    fm->openIdxFile(dbname, tbname, fdname, fileID);
    int index;
    open = true;
    char* cache = fm->getPage(fileID, 0, index);
    ifdp = new IndexFileDescPage(cache, index, 0, true);
    keyType = ifdp->getKeyType();
    keyLength = ifdp->getKeyLength();
    tree = new IndexTree<T>(this, ifdp->getRootPage(), keyLength);
    openPage(ifdp->getRootPage());
}

template<typename T>
void IndexFile<T>::test()
{
    FileIOModel::getInstance()->createDataFile("test", "test");
    IndexFile<IntType> f;
    f.createFile("test", "test", "test", 1, 4, false, 1);
    f.openFile("test", "test", "test");
    for(int i = 0; i < 100; i++)
    {
        for(int j = 1; j < 100; j++)
        {
            assert(f.insert(*(IntType*)&i, j));
        }
    }
    for(int i = 0; i < 100; i++)
    {
        f.printValuesOfKey(*(IntType*)&i);
    }
    for(int i = 0; i < 100; i++)
    {
        for(int j = 1; j < 100; j++)
        {
            assert(f.remove(*(IntType*)&i, j));
        }
    }
    for(int i = 0; i < 100; i++)
    {
        assert(!f.update(*(IntType*)&i, 1, 1));
    }
    FileIOModel::getInstance()->dropTable("test", "test");
}

#endif
