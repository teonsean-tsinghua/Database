#include "DBIndexFile.h"

DBIndexFile::DBIndexFile(){
	fm = DBFileIOModel::getInstance();
	up = NULL;
}

int DBIndexFile::allocateNewPage(){
	DBUsagePage* up = openUsagePage(0);
	int ret = 0;
	while(true){
		ret = up -> findFirstAvailable();
		if (ret > 0)
			break;
		up = openUsagePage(up -> getNextSameType());
	}
	return ret;
}

void DBIndexFile::copyToNewPage(int pageID){
	int newPageID = allocateNewPage();
	(DBIndexDataPage*) pages[newPageID] -> setCache((DBIndexDataPage*)pages[pageID] -> getCache());
}

DBUsagePage* DBIndexFile::openUsagePage(int pid){
    if(pages.count(pid))
    {
        DBPage* re = pages[pid];
        return re->getPageType() == DBType::USAGE_PAGE ? (DBUsagePage*)re : NULL;
    }
    if(pid <= 0 || pid >= dfdp->getPageNumber())
    {
        return NULL;
    }
    int index;
    BufType cache = fm->getPage(fileID, pid, index);
    if(DBPageInfoSlot::getPageType(cache) != DBType::USAGE_PAGE)
    {
        return NULL;
    }
    DBUsagePage* re = new DBUsagePage(cache, index, pid, MODE_PARSE);
    pages[pid] = re;
    return re;
}

DBIndexDataPage* DBIndexFile::openIndexDataPage(int pid){
	if (pages.count(pid)){
		DBPage* re = pages[pid];
		return re -> getPageType == DBType::INDEX_DATA_PAGE ? (DBIndexDataPage*) re : NULL;	
	}
	int index;
	BufType cache = fm -> getPage(fileID, pid, index);
	if (DBPageInfoSlot::getPageType(cache) != DBType::INDEX_DATA_PAGE){
		return NULL;
	}
	DBIndexDataPage* re = new DBIndexDataPage(cache, index, pid, MODE_PARSE);
	pages[pid] = re;
	return re;
}

int DBIndexFile::createFile(const char* name){
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
    ri = new DBRecordInfo();
    dfdp = new DBDataFileDescriptionPage(cache, index, 0, MODE_CREATE, ri);
    dfdp->addField("_id", DBType::_ID, false);
    fm->flush(dfdp->getIndex());
    fm->closeFile(fileID);
    delete ri;
    return SUCCEED;
}

int DBIndexFile::deleteFile(const char* name){
    if(fm->deleteFile(name) != SUCCEED)
    {
        DBLogLine("ERROR");
        return FILE_OR_DIRECTORY_DOES_NOT_EXIST;
    }
    return SUCCEED;
}

int DBIndexFile::closeFile(const char* name){
	fm->flush(dfdp->getIndex());
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

int DBIndexFile::setAvailableOfIndexPage(int ipid, bool available){
	DBUsagePage* up = openUsagePage(0);
	while(true){
		if (up == NULL){
			return ERROR;
		}
		if (up -> withinRange(ipid)){
			break;
		}
		up = openUsagePage(up -> getNextSameType());
	}
	up -> setAvailable(ipid, available);
	return SUCCEED;
}