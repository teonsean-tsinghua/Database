#ifndef DBINDEXFILE_H_INCLUDED
#define DBINDEXFILE_H_INCLUDED
#include "DBIndexDataPage.h"
#include "DBUsagePage.h"

class DBIndexFile{
private:
	DBUsagePage* up;
	DBFileIOModel *fm;
	std::map<int, DBPage*> pages;
	const char* indexname;
	int fileID;
	int dataLen;
public:
	DBIndexFile(const char* name, int _dataLen);

	int allocateNewPage();

	int copyToNewPage(int pageID);

	DBUsagePage* openUsagePage(int pid);

	DBIndexDataPage* openIndexDataPage(int pid);

	int createFile(const char* name);

	int deleteFile(const char* name);

	int closeFile(const char* name);

	int setAvailableOfIndexPage(int ipid, bool available);
};

#endif