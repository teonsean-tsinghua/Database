#include "IndexFile.h"

IndexFile::IndexFile(std::string path):
    path(path)
{
	fm = FileIOModel::getInstance();
    fileID = -1;
    open = false;
}

int IndexFile::allocateNewLeafNode()
{
    assert(open);
    int cnt = ifdp->getPageNumber();
    if(cnt <= 0)
    {
        throw Exception("Invalid page number count.");
    }
    int index;
    BufType cache = fm->getPage(fileID, cnt, index);
    IndexLeafPage* ifp = new IndexLeafPage(cache, index, cnt, false, keyType, keyLength);
    pages[cnt] = ifp;
    Print::log("Allocated new leaf page ").logLine(cnt);
    ifdp->incrementPageNumber();
    return cnt;
}

int IndexFile::allocateNewBucketPage()
{
    assert(open);
    int cnt = ifdp->getPageNumber();
    if(cnt <= 0)
    {
        throw Exception("Invalid page number count.");
    }
    int index;
    BufType cache = fm->getPage(fileID, cnt, index);
    BucketPage* ifp = new BucketPage(cache, index, cnt, false);
    buckets[cnt] = ifp;
    Print::log("Allocated new leaf page ").logLine(cnt);
    ifdp->incrementPageNumber();
    return cnt;
}

int IndexFile::allocateNewInternalNode()
{
    assert(open);
    int cnt = ifdp->getPageNumber();
    if(cnt <= 0)
    {
        throw Exception("Invalid page number count.");
    }
    int index;
    BufType cache = fm->getPage(fileID, cnt, index);
    IndexInternalPage* ifp = new IndexInternalPage(cache, index, cnt, false, keyType, keyLength);
    pages[cnt] = ifp;
    Print::log("Allocated new internal page ").logLine(cnt);
    ifdp->incrementPageNumber();
    return cnt;
}

void IndexFile::printFileDesc()
{
    assert(open);
    ifdp->print();
    Print::print("Max degree of each node is: ").printLine(maxDgr)
            .print("Min degree of each node is: ").printLine(minDgr);
}

IndexNodePage* IndexFile::openNode(int pid)
{
    assert(open);
	if(pages.count(pid))
    {
		return pages[pid];
	}
    if(pid <= 0 || pid >= ifdp->getPageNumber())
    {
        return NULL;
    }
	int index;
	BufType cache = fm->getPage(fileID, pid, index);
	int pageType = PageInfoSlot::getPageType(cache);
    if(pageType == Type::INDEX_INTERNAL_PAGE)
    {
        IndexInternalPage* re = new IndexInternalPage(cache, index, pid, true, keyType, keyLength);
        pages[pid] = re;
        return re;
    }
    else if(pageType == Type::INDEX_LEAF_PAGE)
    {
        IndexLeafPage* re = new IndexLeafPage(cache, index, pid, true, keyType, keyLength);
        pages[pid] = re;
        return re;
    }
    else
    {
        return NULL;
    }
}

void IndexFile::split(IndexNodePage* cur)
{
    assert(open);
    if(cur->getChildrenCount() >= maxDgr)
    {
        int new_pid;
        if(cur->isLeaf())
        {
            new_pid = allocateNewLeafNode();
        }
        else
        {
            new_pid = allocateNewInternalNode();
        }
        IndexNodePage* new_page = openNode(new_pid);
        IndexNodePage::split(cur, new_page);
        new_page->setNextSameType(cur->getNextSameType());
        cur->setNextSameType(new_page->getPageID());
        if(!cur->isLeaf())
        {
            int curPage = new_page->getMinPage();
            while(curPage != new_page->getMaxPage())
            {
                IndexNodePage* tmp = openNode(curPage);
                if(tmp == NULL)
                {
                    throw Exception("Error in index node linked lists.");
                }
                tmp->setParentNode(new_pid);
                curPage = tmp->getNextSameType();
            }
            openNode(curPage)->setParentNode(new_pid);
        }
        if(cur->getParentNode() <= 0)
        {
            int new_root = allocateNewInternalNode();
            rootNode = new_root;
            ifdp->setRootPage(rootNode);
            IndexNodePage* new_root_node = openNode(new_root);
            new_root_node->insert(cur->getMaxKey(), cur->getPageID());
            new_root_node->insert(new_page->getMaxKey(), new_page->getPageID());
            cur->setParentNode(new_root);
            new_page->setParentNode(new_root);
        }
        else
        {
            IndexNodePage* parent = openNode(cur->getParentNode());
            new_page->setParentNode(cur->getParentNode());
            parent->changeKeyOfPage(cur->getPageID(), cur->getMaxKey());
            parent->insert(new_page->getMaxKey(), new_page->getPageID());
            split(parent);
        }
    }
}

void IndexFile::merge(IndexNodePage* cur)
{
    //TODO:
}

void IndexFile::insert(void* key, int pid)
{
    assert(open);
    IndexNodePage* curNode = openNode(rootNode);
    void* maxKey = curNode->getMaxKey();
    if(maxKey == NULL)
    {
        curNode->insert(key, pid);
        return;
    }
    while(true)
    {
        if(curNode == NULL || curNode->isLeaf())
        {
            break;
        }
        if(larger(key, curNode->getMaxKey(), keyType, keyLength))
        {
            while(curNode != NULL && !curNode->isLeaf())
            {
                curNode->setMaxKey(key);
                curNode = openNode(curNode->getMaxPage());
            }
            break;
        }
        curNode = openNode(curNode->search(key));
    }
    if(curNode == NULL)
    {
        throw Exception("Invalid page ID.");
    }
    curNode->insert(key, pid);
    split(curNode);
}

void IndexFile::remove(void* key)
{
    assert(open);
    IndexNodePage* curNode = openNode(rootNode);
    while(curNode != NULL && !curNode->isLeaf())
    {
        curNode = openNode(curNode->search(key));
    }
    if(curNode == NULL)
    {
        throw Exception("Invalid page ID.");
    }
    curNode->remove(key);
    merge(curNode);
}

int IndexFile::search(void* key)
{
    assert(open);
    IndexNodePage* curNode = openNode(rootNode);
    while(curNode != NULL && !curNode->isLeaf())
    {
        curNode = openNode(curNode->search(key));
    }
    if(curNode == NULL)
    {
        throw Exception("Invalid page ID.");
    }
    return curNode->searchEqual(key);
}

void IndexFile::update(void* key, int pid)
{
    assert(open);
    IndexNodePage* curNode = openNode(rootNode);
    while(curNode != NULL && !curNode->isLeaf())
    {
        curNode = openNode(curNode->search(key));
    }
    if(curNode == NULL)
    {
        throw Exception("Invalid page ID.");
    }
    curNode->update(key, pid);
}

void IndexFile::createFile(int keyType, int keyLength, bool unique)
{
    assert(!open);
    fm->createFile(path.c_str());
    fm->openFile(path.c_str(), fileID);
    this->keyType = keyType;
    this->keyLength = keyLength;
    open = true;
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    ifdp = new IndexFileDescPage(cache, index, 0, false, keyType, keyLength);
    int root = allocateNewLeafNode();
    ifdp->setRootPage(root);
    ifdp->setUnique(unique);
    fm->flush(ifdp->getIndex());
    fm->closeFile(fileID);
    open = false;
}

void IndexFile::deleteFile()
{
    assert(!open);
    fm->deleteFile(path.c_str());
}

void IndexFile::closeFile()
{
    assert(open);
    fm->flush(ifdp->getIndex());
    for(std::map<int, IndexNodePage*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
    {
        fm->flush(iter->second->getIndex());
    }
    for(std::map<int, BucketPage*>::iterator iter = buckets.begin(); iter != buckets.end(); iter++)
    {
        fm->flush(iter->second->getIndex());
    }
    pages.clear();
    buckets.clear();
    open = false;
    fm->closeFile(fileID);
}

void IndexFile::openFile()
{
    assert(!open);
    fm->openFile(path.c_str(), fileID);
    int index;
    open = true;
    BufType cache = fm->getPage(fileID, 0, index);
    ifdp = new IndexFileDescPage(cache, index, 0, true);
    keyType = ifdp->getKeyType();
    rootNode = ifdp->getRootPage();
    keyLength = ifdp->getKeyLength();
    openNode(rootNode)->calcDegree(minDgr, maxDgr);
}

void IndexFile::_test()
{
    createFile(Type::INT, Type::typeSize(Type::INT), true);
    openFile();
//    for(int i = 0; i < 1000000; i++)
//    {
//        insert(&i, i);
//    }
//    closeFile();
//
//    openFile("test.idx");
//    printFileDesc();
//    for(int i = 0; i < 1000000; i++)
//    {
//        PrintLine(search(&i));
//    }
//    int k = 827046620;
//    remove(&k);
    std::map<int, int> m;
    for(int i = 0; i < 1021; i++)
    {
        int k = i;
        if(!m.count(k))
        {
            insert(&k, k);
        }
    }
    for(int i = 1; i < ifdp->getPageNumber(); i++)
    {
        openNode(i)->print();
    }
    closeFile();
}

void IndexFile::test()
{
    IndexFile id("test.idx");
    id._test();
}
