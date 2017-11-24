#ifndef DBINDEXFILE_H_INCLUDED
#define DBINDEXFILE_H_INCLUDED
#include "../DBInclude.h"
#include "DBIndexDataPage.h"
#include "DBIndexUsagePage.h"

class DBIndexFile{
private:
	DBIndexUsagePage* iup;
	DBFileIOModel *fm;
	std::map<int, DBPage*> pages;
	const char* indexname;
	int fileID;
	int dataLen;
	int lastIndexUsagePage;
	bool isFileOpen;
public:
	DBIndexFile(const char* name, int _dataLen);

	int allocateNewIndexPage();

	int allocateNewUsagePage();

	int copyToNewPage(int pageID);

	DBIndexUsagePage* openIndexUsagePage(int pid);

	DBIndexDataPage* openIndexDataPage(int pid);

	int setAvailableOfIndexPage(int ipid, bool available);

	int createFile();

	int openFile();

	int deleteFile();

	int closeFile();
};

#endif