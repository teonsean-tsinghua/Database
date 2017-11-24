#include "DBIndexFile.h"

DBIndexFile::DBIndexFile(const char* root, int _dataLen):
    root(root),
{
	fm = DBFileIOModel::getInstance();
    fileID = -1;
    open = false;
}

int DBIndexFile::allocateNewIndexPage()
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
	iup = openIndexUsagePage(lastIndexUsagePage);
	int ret = 0;
	while(true){
		ret = iup -> findFirstAvailable();
		if (ret > 0){
            iup -> setAvailable(ret, false);
			break;
        }
        if (iup -> getNextSameType() != -1)
		    iup = openIndexUsagePage(iup -> getNextSameType());
        else{
            allocateNewUsagePage();
            iup = openIndexUsagePage(lastIndexUsagePage);
        }
	}
    BufType _cache = new unsigned int[PAGE_SIZE >> 2];
    DBIndexDataPage* re = new DBIndexDataPage(_cache, ret, ret, MODE_CREATE, dataLen);
    pages[ret] = re;
    std::cout << "allocate new index page at " << ret << endl;
	return ret;
}

int DBIndexFile::copyToNewPage(int pageID)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
	int newPageID = allocateNewIndexPage();
    DBIndexDataPage* newPage = (DBIndexDataPage*) pages[newPageID];
    DBIndexDataPage* curpage = (DBIndexDataPage*)pages[pageID];
	newPage -> setCache(curpage -> getCache());
    this -> setAvailableOfIndexPage(newPageID, false);
    return newPageID;
}

DBIndexDataPage* DBIndexFile::openIndexDataPage(int pid)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
    if (isFileOpen == false)
        openFile();
	if (pages.count(pid)){
		DBPage* re = pages[pid];
		return re -> getPageType() == DBType::INDEX_DATA_PAGE ? (DBIndexDataPage*) re : NULL;
	}
	int index;
	BufType cache = fm -> getPage(fileID, pid, index);
	DBIndexDataPage* re = new DBIndexDataPage(cache, index, pid, MODE_PARSE, dataLen);
    re -> print();
	pages[pid] = re;
    std::cout << "allocate new index data page at " << pid << endl;
	return re;
}

int DBIndexFile::createFile()
{
    if(open)
    {
        DBLogLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->createFile(indexname) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    if(fm->openFile(indexname, fileID) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    int index;
    BufType cache = fm->getPage(fileID, 0, index);
    DBUsagePage* us = new DBUsagePage(cache, 0, 0, MODE_CREATE);
    cache = fm -> getPage(fileID, 1, index);
    DBIndexDataPage* idp = new DBIndexDataPage(cache, 1, 1, MODE_CREATE, 5);
    fm -> flush(us -> getIndex());
    fm -> flush(idp -> getIndex());
//    fm -> closeFile(fileID);
    return SUCCEED;
}

int DBIndexFile::deleteFile(){
    if(open)
    {
        DBLogLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->deleteFile(indexname) != SUCCEED)
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
    lastIndexUsagePage = -1;
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

int DBIndexFile::setAvailableOfIndexPage(int ipid, bool available)
{
    if(!open)
    {
        return FILE_NOT_OPENED;
    }
	DBUsagePage* up = openIndexUsagePage(0);
	while(true){
		if (up == NULL){
			return ERROR;
		}
		if (up -> withinRange(ipid)){
			break;
		}
		up = openIndexUsagePage(up -> getNextSameType());
	}
	up -> setAvailable(ipid, available);
	return SUCCEED;
}

int DBIndexFile::openFile(){
    if(open)
    {
        DBLogLine("ERROR");
        return A_FILE_ALREADY_OPENED;
    }
    if(fm->openFile(indexname, fileID) != SUCCEED){
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    return SUCCEED;
}
