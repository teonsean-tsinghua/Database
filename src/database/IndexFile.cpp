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
    assert(cnt > 0);
    int index;
    BufType cache = fm->getPage(fileID, cnt, index);
    LeafPage* ifp = new LeafPage(cache, index, cnt, keyType, keyLength, false);
    pages[cnt] = ifp;
    Print::log("Allocated new leaf page ").logLine(cnt);
    ifdp->incrementPageNumber(Type::LEAF_PAGE);
    return cnt;
}

int IndexFile::allocateNewInternalNode()
{
    assert(open);
    int cnt = ifdp->getPageNumber();
    assert(cnt > 0);
    int index;
    BufType cache = fm->getPage(fileID, cnt, index);
    InternalPage* ifp = new InternalPage(cache, index, cnt, keyType, keyLength, false);
    pages[cnt] = ifp;
    Print::log("Allocated new internal page ").logLine(cnt);
    ifdp->incrementPageNumber(Type::INTERNAL_PAGE);
    return cnt;
}

void IndexFile::printFileDesc()
{
    assert(open);
    ifdp->print();
//    Print::print("Max degree of each node is: ").printLine(maxDgr)
//            .print("Min degree of each node is: ").printLine(minDgr);
}

InternalPage* IndexFile::openInternalPage(int pid)
{
    Page* re = openPage(pid);
    assert(re->getPageType() == Type::INTERNAL_PAGE);
    return (InternalPage*)re;
}

LeafPage* IndexFile::openLeafPage(int pid)
{
    Page* re = openPage(pid);
    assert(re->getPageType() == Type::LEAF_PAGE);
    return (LeafPage*)re;
}

UsagePage* IndexFile::openUsagePage(int pid)
{
    Page* re = openPage(pid);
    assert(re->getPageType() == Type::USAGE_PAGE);
    return (UsagePage*)re;
}

NodePage* IndexFile::openNodePage(int pid)
{
    Page* re = openPage(pid);
    assert(re->getPageType() == Type::INTERNAL_PAGE || re->getPageType() == Type::LEAF_PAGE);
    return (NodePage*)re;
}

BucketPage* IndexFile::openBucketPage(int pid)
{
    Page* re = openPage(pid);
    assert(re->getPageType() == Type::BUCKET_PAGE);
    return (BucketPage*)re;
}

Page* IndexFile::openPage(int pid)
{
    assert(open);
	if(pages.count(pid))
    {
		return pages[pid];
	}
    assert(pid > 0 && pid < ifdp->getPageNumber());
	int index;
	BufType cache = fm->getPage(fileID, pid, index);
	int pageType = PageInfoSlot::getPageType(cache);
	switch(pageType)
	{
    case Type::INTERNAL_PAGE:
    {
        InternalPage* re = new InternalPage(cache, index, pid, keyType, keyLength, true);
        pages[pid] = re;
        return re;
    }
    case Type::LEAF_PAGE:
    {
        LeafPage* re  = new LeafPage(cache, index, pid, keyType, keyLength, true);
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
        BucketPage* re  = new BucketPage(cache, index, pid, true, density);
        pages[pid] = re;
        return re;
    }
    default:
        assert(false);
    }
    return NULL;
}

//void IndexFile::split(IndexNodePage* cur)
//{
//    assert(open);
//    if(cur->getChildrenCount() >= maxDgr)
//    {
//        int new_pid;
//        if(cur->isLeaf())
//        {
//            new_pid = allocateNewLeafNode();
//        }
//        else
//        {
//            new_pid = allocateNewInternalNode();
//        }
//        IndexNodePage* new_page = openNode(new_pid);
//        IndexNodePage::split(cur, new_page);
//        new_page->setNextSameType(cur->getNextSameType());
//        cur->setNextSameType(new_page->getPageID());
//        if(!cur->isLeaf())
//        {
//            int curPage = new_page->getMinPage();
//            while(curPage != new_page->getMaxPage())
//            {
//                IndexNodePage* tmp = openNode(curPage);
//                if(tmp == NULL)
//                {
//                    throw Exception("Error in index node linked lists.");
//                }
//                tmp->setParentNode(new_pid);
//                curPage = tmp->getNextSameType();
//            }
//            openNode(curPage)->setParentNode(new_pid);
//        }
//        if(cur->getParentNode() <= 0)
//        {
//            int new_root = allocateNewInternalNode();
//            rootNode = new_root;
//            ifdp->setRootPage(rootNode);
//            IndexNodePage* new_root_node = openNode(new_root);
//            new_root_node->insert(cur->getMaxKey(), cur->getPageID());
//            new_root_node->insert(new_page->getMaxKey(), new_page->getPageID());
//            cur->setParentNode(new_root);
//            new_page->setParentNode(new_root);
//        }
//        else
//        {
//            IndexNodePage* parent = openNode(cur->getParentNode());
//            new_page->setParentNode(cur->getParentNode());
//            parent->changeKeyOfPage(cur->getPageID(), cur->getMaxKey());
//            parent->insert(new_page->getMaxKey(), new_page->getPageID());
//            split(parent);
//        }
//    }
//}
//
//void IndexFile::merge(IndexNodePage* cur)
//{
//    //TODO:
//}
//
//void IndexFile::insert(void* key, int pid)
//{
//    assert(open);
//    IndexNodePage* curNode = openNode(rootNode);
//    void* maxKey = curNode->getMaxKey();
//    if(maxKey == NULL)
//    {
//        curNode->insert(key, pid);
//        return;
//    }
//    while(true)
//    {
//        if(curNode == NULL || curNode->isLeaf())
//        {
//            break;
//        }
//        if(larger(key, curNode->getMaxKey(), keyType, keyLength))
//        {
//            while(curNode != NULL && !curNode->isLeaf())
//            {
//                curNode->setMaxKey(key);
//                curNode = openNode(curNode->getMaxPage());
//            }
//            break;
//        }
//        curNode = openNode(curNode->search(key));
//    }
//    if(curNode == NULL)
//    {
//        throw Exception("Invalid page ID.");
//    }
//    curNode->insert(key, pid);
//    split(curNode);
//}
//
//void IndexFile::remove(void* key)
//{
//    assert(open);
//    IndexNodePage* curNode = openNode(rootNode);
//    while(curNode != NULL && !curNode->isLeaf())
//    {
//        curNode = openNode(curNode->search(key));
//    }
//    if(curNode == NULL)
//    {
//        throw Exception("Invalid page ID.");
//    }
//    curNode->remove(key);
//    merge(curNode);
//}
//
//int IndexFile::search(void* key)
//{
//    assert(open);
//    IndexNodePage* curNode = openNode(rootNode);
//    while(curNode != NULL && !curNode->isLeaf())
//    {
//        curNode = openNode(curNode->search(key));
//    }
//    if(curNode == NULL)
//    {
//        throw Exception("Invalid page ID.");
//    }
//    return curNode->searchEqual(key);
//}
//
//void IndexFile::update(void* key, int pid)
//{
//    assert(open);
//    IndexNodePage* curNode = openNode(rootNode);
//    while(curNode != NULL && !curNode->isLeaf())
//    {
//        curNode = openNode(curNode->search(key));
//    }
//    if(curNode == NULL)
//    {
//        throw Exception("Invalid page ID.");
//    }
//    curNode->update(key, pid);
//}

void IndexFile::createFile(std::string dbname, std::string tbname, std::string fdname, int keyType, int keyLength, bool unique, int density = 1)
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
    int root = allocateNewLeafNode();
    ifdp->setRootPage(root);
    ifdp->setUnique(unique);
    ifdp->setDensity(density);
    fm->flush(ifdp->getIndex());
    fm->closeFile(fileID);
    open = false;
    delete ifdp;
}

void IndexFile::closeFile()
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

void IndexFile::openFile(std::string dbname, std::string tbname, std::string fdname)
{
    assert(!open);
    fm->openIdxFile(dbname, tbname, fdname);
    int index;
    open = true;
    char* cache = fm->getPage(fileID, 0, index);
    ifdp = new IndexFileDescPage(cache, index, 0, true);
    keyType = ifdp->getKeyType();
    keyLength = ifdp->getKeyLength();
    lastBucketPage = lastUsagePage = -1;
    openNode(ifdp->getRootPage());
}

//void IndexFile::_test()
//{
//    createFile(Type::INT, Type::typeSize(Type::INT));
//    openFile();
////    for(int i = 0; i < 1000000; i++)
////    {
////        insert(&i, i);
////    }
////    closeFile();
////
////    openFile("test.idx");
////    printFileDesc();
////    for(int i = 0; i < 1000000; i++)
////    {
////        PrintLine(search(&i));
////    }
////    int k = 827046620;
////    remove(&k);
//    std::map<int, int> m;
//    for(int i = 0; i < 5000; i++)
//    {
//        int k = rand();
//        if(!m.count(k))
//        {
//            int p = rand();
//            insert(&k, p);
//        }
//    }
//    for(int i = 1; i < ifdp->getPageNumber(); i++)
//    {
//        openNode(i)->print();
//    }
//    closeFile();
//}
//
//void IndexFile::test()
//{
//    IndexFile id("test.idx");
//    id._test();
//}
