#include "DBIndexFile.h"

DBIndexFile::DBIndexFile(const char* root):
    root(root)
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
    DBIndexLeafPage* ifp = new DBIndexLeafPage(cache, index, cnt, MODE_CREATE, keyType);
    pages[cnt] = ifp;
    DBLog("Allocated new leaf page ");
    DBLogLine(cnt);
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
    DBIndexInternalPage* ifp = new DBIndexInternalPage(cache, index, cnt, MODE_CREATE, keyType);
    pages[cnt] = ifp;
    DBLog("Allocated new internal page ");
    DBLogLine(cnt);
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
    DBPrint("Max degree of each node is: ");
    DBPrintLine(maxDgr);
    DBPrint("Min degree of each node is: ");
    DBPrintLine(minDgr);
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
        DBIndexInternalPage* re = new DBIndexInternalPage(cache, index, pid, MODE_PARSE, keyType);
        pages[pid] = re;
        return re;
    }
    else if(pageType == DBType::INDEX_LEAF_PAGE)
    {
        DBIndexLeafPage* re = new DBIndexLeafPage(cache, index, pid, MODE_PARSE, keyType);
        pages[pid] = re;
        return re;
    }
    else
    {
        return NULL;
    }
}

int DBIndexFile::insert(void* key, int pid)
{
    DBIndexNodePage* curNode = openNode(rootNode);
    if(curNode == NULL)
    {
        return ERROR;
    }
    void* maxKey = curNode->getMaxKey();
    if(maxKey == NULL)
    {
        curNode->insert(key, pid);
        return SUCCEED;
    }
    if(larger(key, maxKey, keyType))
    {
        while(curNode != NULL && !curNode->isLeaf())
        {
            curNode = openNode(curNode->getMaxPage());
        }
    }
    else
    {
        while(curNode != NULL && !curNode->isLeaf())
        {
            curNode = openNode(curNode->search(key));
        }
    }
    if(curNode == NULL)
    {
        return ERROR;
    }
    curNode->insert(key, pid);
    return SUCCEED;
}

int DBIndexFile::createFile(const char* name, int keyType)
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
    this->keyType = keyType;
    open = true;
    keyLength = DBType::typeSize(keyType);
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    ifdp = new DBIndexFileDescriptionPage(cache, index, 0, MODE_CREATE, keyType);
    int root = allocateNewLeafNode();
    ifdp->setRootPage(root);
    fm->flush(ifdp->getIndex());
    fm->closeFile(fileID);
    open = false;
    return SUCCEED;
}

int DBIndexFile::deleteFile(const char* name)
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

int DBIndexFile::closeFile()
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    for(std::map<int, DBIndexNodePage*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
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

int DBIndexFile::openFile(const char* name)
{
    if(open)
    {
        DBLogLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->openFile(name, fileID) != SUCCEED){
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    int index;
    open = true;
    this->name = name;
    BufType cache = fm->getPage(fileID, 0, index);
    ifdp = new DBIndexFileDescriptionPage(cache, index, 0, MODE_PARSE);
    keyType = ifdp->getKeyType();
    rootNode = ifdp->getRootPage();
    keyLength = DBType::typeSize(keyType);
    openNode(rootNode)->calcDegree(minDgr, maxDgr);
    return SUCCEED;
}

void DBIndexFile::test()
{
    createFile("test.idx", DBType::INT);
    closeFile();
    openFile("test.idx");
    openNode(rootNode)->print();
    int k = 1;
    insert(&k, 10);
    int k2 = -2;
    insert(&k2, 20);
    int k3 = -5;
    insert(&k3, 1);
    int k4 = 5;
    insert(&k4, 980);
    DBPrintLine(openNode(rootNode)->search(&k));
    DBPrintLine(openNode(rootNode)->search(&k2));
    DBPrintLine(openNode(rootNode)->search(&k3));
    DBPrintLine(openNode(rootNode)->search(&k4));
    closeFile();
}
