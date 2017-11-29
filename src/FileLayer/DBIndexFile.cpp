#include "DBIndexFile.h"

DBIndexFile::DBIndexFile(const char* path):
    path(path)
{
	fm = DBFileIOModel::getInstance();
    fileID = -1;
    open = false;
}

int DBIndexFile::allocateNewLeafNode()
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    int cnt = ifdp->getPageNumber();
    if(cnt <= 0)
    {
        return ERROR;
    }
    int index;
    BufType cache = fm->getPage(fileID, cnt, index);
    DBIndexLeafPage* ifp = new DBIndexLeafPage(cache, index, cnt, MODE_CREATE, keyType, keyLength);
    pages[cnt] = ifp;
    DBPrint::log("Allocated new leaf page ").logLine(cnt);
    ifdp->incrementPageNumber();
    return cnt;
}

int DBIndexFile::allocateNewInternalNode()
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    int cnt = ifdp->getPageNumber();
    if(cnt <= 0)
    {
        return ERROR;
    }
    int index;
    BufType cache = fm->getPage(fileID, cnt, index);
    DBIndexInternalPage* ifp = new DBIndexInternalPage(cache, index, cnt, MODE_CREATE, keyType, keyLength);
    pages[cnt] = ifp;
    DBPrint::log("Allocated new internal page ").logLine(cnt);
    ifdp->incrementPageNumber();
    return cnt;
}

void DBIndexFile::printFileDescription()
{
    if(!open)
    {
        return;
    }
    ifdp->print();
    DBPrint::print("Max degree of each node is: ").printLine(maxDgr)
            .print("Min degree of each node is: ").printLine(minDgr);
}

DBIndexNodePage* DBIndexFile::openNode(int pid)
{
    if(!open)
    {
        return NULL;
    }
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
	int pageType = DBPageInfoSlot::getPageType(cache);
    if(pageType == DBType::INDEX_INTERNAL_PAGE)
    {
        DBIndexInternalPage* re = new DBIndexInternalPage(cache, index, pid, MODE_PARSE, keyType, keyLength);
        pages[pid] = re;
        return re;
    }
    else if(pageType == DBType::INDEX_LEAF_PAGE)
    {
        DBIndexLeafPage* re = new DBIndexLeafPage(cache, index, pid, MODE_PARSE, keyType, keyLength);
        pages[pid] = re;
        return re;
    }
    else
    {
        return NULL;
    }
}

int DBIndexFile::split(DBIndexNodePage* cur)
{
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
        if(new_pid <= 0)
        {
            return ERROR;
        }
        DBIndexNodePage* new_page = openNode(new_pid);
        DBIndexNodePage::split(cur, new_page);
        new_page->setNextSameType(cur->getNextSameType());
        cur->setNextSameType(new_page->getPageID());
        if(!cur->isLeaf())
        {
            int curPage = new_page->getMinPage();
            while(curPage != new_page->getMaxPage())
            {
                DBIndexNodePage* tmp = openNode(curPage);
                if(tmp == NULL)
                {
                    DBPrint::log("Failed to open page ").logLine(new_pid);
                    return ERROR;
                }
                tmp->setParent(new_pid);
                curPage = tmp->getNextSameType();
            }
            openNode(curPage)->setParent(new_pid);
        }
        if(cur->getParent() <= 0)
        {
            int new_root = allocateNewInternalNode();
            if(new_root <= 0)
            {
                return ERROR;
            }
            rootNode = new_root;
            ifdp->setRootPage(rootNode);
            DBIndexNodePage* new_root_node = openNode(new_root);
            new_root_node->insert(cur->getMaxKey(), cur->getPageID());
            new_root_node->insert(new_page->getMaxKey(), new_page->getPageID());
            cur->setParent(new_root);
            new_page->setParent(new_root);
            return SUCCEED;
        }
        else
        {
            DBIndexNodePage* parent = openNode(cur->getParent());
            new_page->setParent(cur->getParent());
            parent->changeKeyOfPage(cur->getPageID(), cur->getMaxKey());
            parent->insert(new_page->getMaxKey(), new_page->getPageID());
            return split(parent);
        }
    }
    return SUCCEED;
}

int DBIndexFile::merge(DBIndexNodePage* cur)
{
    //TODO:
}

int DBIndexFile::insert(void* key, int pid)
{
    DBIndexNodePage* curNode = openNode(rootNode);
    void* maxKey = curNode->getMaxKey();
    if(maxKey == NULL)
    {
        curNode->insert(key, pid);
        return SUCCEED;
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
        return ERROR;
    }
    int re = curNode->insert(key, pid);
    if(re != SUCCEED)
    {
        return re;
    }
    return split(curNode);
}

int DBIndexFile::remove(void* key)
{
    DBIndexNodePage* curNode = openNode(rootNode);
    while(curNode != NULL && !curNode->isLeaf())
    {
        curNode = openNode(curNode->search(key));
    }
    if(curNode == NULL)
    {
        return ERROR;
    }
    int re = curNode->remove(key);
    if(re != SUCCEED)
    {
        return re;
    }
    return merge(curNode);
}

int DBIndexFile::search(void* key)
{
    DBIndexNodePage* curNode = openNode(rootNode);
    while(curNode != NULL && !curNode->isLeaf())
    {
        curNode = openNode(curNode->search(key));
    }
    if(curNode == NULL)
    {
        return ERROR;
    }
    return curNode->searchEqual(key);
}

int DBIndexFile::update(void* key, int pid)
{
    DBIndexNodePage* curNode = openNode(rootNode);
    while(curNode != NULL && !curNode->isLeaf())
    {
        curNode = openNode(curNode->search(key));
    }
    if(curNode == NULL)
    {
        return ERROR;
    }
    return curNode->update(key, pid);
}

int DBIndexFile::createFile(int keyType, int keyLength)
{
    if(open)
    {
        DBPrint::logLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->createFile(path) != SUCCEED)
    {
        DBPrint::logLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    if(fm->openFile(path, fileID) != SUCCEED)
    {
        DBPrint::logLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    this->keyType = keyType;
    this->keyLength = keyLength;
    open = true;
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    ifdp = new DBIndexFileDescriptionPage(cache, index, 0, MODE_CREATE, keyType, keyLength);
    int root = allocateNewLeafNode();
    ifdp->setRootPage(root);
    fm->flush(ifdp->getIndex());
    fm->closeFile(fileID);
    open = false;
    return SUCCEED;
}

int DBIndexFile::deleteFile()
{
    if(open)
    {
        DBPrint::logLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->deleteFile(path) != SUCCEED)
    {
        DBPrint::logLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    return SUCCEED;
}

int DBIndexFile::closeFile()
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    fm->flush(ifdp->getIndex());
    for(std::map<int, DBIndexNodePage*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
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

int DBIndexFile::openFile()
{
    if(open)
    {
        DBPrint::logLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->openFile(path, fileID) != SUCCEED){
        DBPrint::logLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    int index;
    open = true;
    BufType cache = fm->getPage(fileID, 0, index);
    ifdp = new DBIndexFileDescriptionPage(cache, index, 0, MODE_PARSE);
    keyType = ifdp->getKeyType();
    rootNode = ifdp->getRootPage();
    keyLength = ifdp->getKeyLength();
    openNode(rootNode)->calcDegree(minDgr, maxDgr);
    return SUCCEED;
}

void DBIndexFile::_test()
{
    createFile(DBType::INT, DBType::typeSize(DBType::INT));
    openFile();
//    for(int i = 0; i < 1000000; i++)
//    {
//        insert(&i, i);
//    }
//    closeFile();
//
//    openFile("test.idx");
//    printFileDescription();
//    for(int i = 0; i < 1000000; i++)
//    {
//        DBPrintLine(search(&i));
//    }
//    int k = 827046620;
//    remove(&k);
    std::map<int, int> m;
    for(int i = 0; i < 5000; i++)
    {
        int k = rand();
        if(!m.count(k))
        {
            int p = rand();
            insert(&k, p);
        }
    }
    for(int i = 1; i < ifdp->getPageNumber(); i++)
    {
        openNode(i)->print();
    }
    closeFile();
}

void DBIndexFile::test()
{
    DBIndexFile id("test.idx");
    id._test();
}
