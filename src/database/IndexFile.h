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
    int lastUsagePage;
    int lastBucketPage;
    IndexFileDescPage* ifdp;
    IndexTree<T>* tree;
    std::map<int, Page*> pages;
    bool open;

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

};

template<typename T>
IndexFile<T>::IndexFile()
{
	fm = FileIOModel::getInstance();
    fileID = -1;
    open = false;
}

template<typename T>
void IndexFile<T>::setRootPage(int n)
{
    ifdp->setRootPage(n);
}

template<typename T>
void IndexFile<T>::markAsUsable(int n)
{
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
    std::cout << "Allocated new usage page " << cnt << std::endl;
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
        std::cout << "Allocated new leaf page " << cnt << std::endl;
        ifdp->incrementPageNumber();
        if(ifdp->getFirstLeafPage() < 0)
        {
            ifdp->setFirstLeafPage(cnt);
        }
    }
    else
    {
        pages[cnt] = new InternalPage<T>(cache, index, cnt, keyType, keyLength, false);
        std::cout << "Allocated new internal page " << cnt << std::endl;
        ifdp->incrementPageNumber();
    }
    up->extendRange(cnt);
    up->setAvailable(cnt, true);
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
    std::cout << "Allocated new bucket page " << cnt << std::endl;
    ifdp->incrementPageNumber();
    if(ifdp->getFirstBucketPage() < 0)
    {
        ifdp->setFirstBucketPage(-1);
        ifdp->setLastBucketPage(-1);
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
    ifdp->setRootPage(ifdp->getRootPage());
    ifdp->setUnique(unique);
    ifdp->setDensity(density);
    int root = allocateNodePage(true);
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
    lastBucketPage = lastUsagePage = -1;
    openPage(ifdp->getRootPage());
}

#endif
