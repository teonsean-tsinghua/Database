#ifndef DBINDEXFILE_H_INCLUDED
#define DBINDEXFILE_H_INCLUDED
#include "../DBInclude.h"
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
	int lastUsagePage;
public:
	DBIndexFile(const char* name);

	int allocateNewIndexPage();

	int allocateNewUsagePage();

	int copyToNewPage(int pageID);

	DBUsagePage* openUsagePage(int pid);

	DBIndexDataPage* openIndexDataPage(int pid);

	int setAvailableOfIndexPage(int ipid, bool available);

	int createFile(const char* name);

	int openFile(const char* name);

	int deleteFile(const char* name);

	int closeFile();
};

#endif