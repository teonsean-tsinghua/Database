#ifndef DBINDEXFILE_H_INCLUDED
#define DBINDEXFILE_H_INCLUDED
#include "../DBInclude.h"
#include "DBIndexFileDescriptionPage.h"
#include "DBUsagePage.h"

class DBIndexFile{
private:
    int fileID;
    int keyType;
    const char* root;
    DBFileIOModel* fm;
    DBIndexFileDescriptionPage* ifdp;
    std::map<int, DBPage*> pages;
    const char* name;
    bool open;

public:
	DBIndexFile(const char* root);

	int allocateNewIndexPage();

	int copyToNewPage(int pageID);

//	DBIndexDataPage* openIndexDataPage(int pid);

	int createFile(const char* name, int keyType);

	int openFile(const char* name);

	int deleteFile(const char* name);

	int closeFile();
};

#endif
