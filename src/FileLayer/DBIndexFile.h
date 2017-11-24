#ifndef DBINDEXFILE_H_INCLUDED
#define DBINDEXFILE_H_INCLUDED
#include "../DBInclude.h"
#include "DBIndexDataPage.h"
#include "DBUsagePage.h"

class DBIndexFile{
private:
    int fileID;
    const char* root;
    DBFileIOModel* fm;
    DBIndexFileDescriptionPage* ifdp;
    std::map<int, DBPage*> pages;
    const char* name;
    bool open;

public:
	DBIndexFile(const char* root, const char* name, int _dataLen);

	int allocateNewIndexPage();

	int allocateNewUsagePage();

	int copyToNewPage(int pageID);

	DBIndexDataPage* openIndexDataPage(int pid);

	int setAvailableOfIndexPage(int ipid, bool available);

	int createFile();

	int openFile();

	int deleteFile();

	int closeFile();
};

#endif
