#ifndef DBINDEXFILE_H_INCLUDED
#define DBINDEXFILE_H_INCLUDED
#include "DBIndexDataPage.h"

class DBIndexFile{
private:
	DBUsagePage* up;
	DBFileIOModel *fm;
	std::map<int, DBPage*> pages;
protected:
	DBIndexFile();

	int allocateNewPage();

	void copyToNewPage(int pageID);

	DBUsagePage* openUsagePage();

	DBIndexDataPage* openIndexDataPage();

	int createFile(const char* name);

	int deleteFile(const char* name);

	int closeFile(const char* name);

	int setAvailableOfIndexPage(int ipid, bool available);
};

#endif