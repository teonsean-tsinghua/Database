#include "DBIndexFile.h"

DBIndexFile::DBIndexFile(const char* root):
    root(root)
{
	fm = DBFileIOModel::getInstance();
    fileID = -1;
    open = false;
}

int DBIndexFile::allocateNewIndexPage()
{
//    if(!open)
//    {
//        return FILE_NOT_OPENED;
//    }
//	iup = openIndexUsagePage(lastIndexUsagePage);
//	int ret = 0;
//	while(true){
//		ret = iup->findFirstAvailable();
//		if (ret > 0){
//            iup->setAvailable(ret, false);
//			break;
//        }
//        if (iup->getNextSameType() != -1)
//		    iup = openIndexUsagePage(iup->getNextSameType());
//        else{
//            allocateNewUsagePage();
//            iup = openIndexUsagePage(lastIndexUsagePage);
//        }
//	}
//    BufType _cache = new unsigned int[PAGE_SIZE >> 2];
//    DBIndexDataPage* re = new DBIndexDataPage(_cache, ret, ret, MODE_CREATE, dataLen);
//    pages[ret] = re;
//    std::cout << "allocate new index page at " << ret << endl;
//	return ret;
}

int DBIndexFile::copyToNewPage(int pageID)
{
//    if(!open)
//    {
//        return FILE_NOT_OPENED;
//    }
//	int newPageID = allocateNewIndexPage();
//    DBIndexDataPage* newPage = (DBIndexDataPage*) pages[newPageID];
//    DBIndexDataPage* curpage = (DBIndexDataPage*)pages[pageID];
//	newPage->setCache(curpage->getCache());
//    this->setAvailableOfIndexPage(newPageID, false);
//    return newPageID;
}

//DBIndexDataPage* DBIndexFile::openIndexDataPage(int pid)
//{
//    if(!open)
//    {
//        return FILE_NOT_OPENED;
//    }
//    if (isFileOpen == false)
//        openFile();
//	if (pages.count(pid)){
//		DBPage* re = pages[pid];
//		return re->getPageType() == DBType::INDEX_DATA_PAGE ? (DBIndexDataPage*) re : NULL;
//	}
//	int index;
//	BufType cache = fm->getPage(fileID, pid, index);
//	DBIndexDataPage* re = new DBIndexDataPage(cache, index, pid, MODE_PARSE, dataLen);
//    re->print();
//	pages[pid] = re;
//    std::cout << "allocate new index data page at " << pid << endl;
//	return re;
//}

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
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    ifdp = new DBIndexFileDescriptionPage(cache, index, 0, MODE_CREATE, keyType);
    fm->flush(ifdp->getIndex());
    fm->closeFile(fileID);
    return SUCCEED;
}

int DBIndexFile::deleteFile(const char* name){
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
    for(std::map<int, DBPage*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
    {
        fm->flush(iter->second->getIndex());
    }
    pages.clear();
    if(fm->closeFile(fileID) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    return SUCCEED;
}

int DBIndexFile::openFile(const char* name){
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
    this->name = name;
    BufType cache = fm->getPage(fileID, 0, index);
    ifdp = new DBIndexFileDescriptionPage(cache, index, 0, MODE_PARSE);
    keyType = ifdp->getKeyType();
    ifdp->print();
    return SUCCEED;
}
