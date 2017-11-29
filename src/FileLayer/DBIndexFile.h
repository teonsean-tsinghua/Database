#ifndef DBINDEXFILE_H_INCLUDED
#define DBINDEXFILE_H_INCLUDED
#include "../DBInclude.h"
#include "DBIndexFileDescriptionPage.h"
#include "DBIndexLeafPage.h"
#include "DBIndexInternalPage.h"
#include "DBUsagePage.h"

class DBIndexFile{
private:
    int fileID;
    int keyLength;
    int keyType;
    const char* root;
    DBFileIOModel* fm;
    DBIndexFileDescriptionPage* ifdp;
    std::map<int, DBIndexNodePage*> pages;
    int rootNode;
    int minDgr;
    int maxDgr;
    const char* name;
    bool open;

    int split(DBIndexNodePage* cur);

    int merge(DBIndexNodePage* cur);

    void _test();

public:
	DBIndexFile(const char* root);

	int allocateNewLeafNode();

	int allocateNewInternalNode();

	DBIndexNodePage* openNode(int pid);

	int createFile(const char* name, int keyType, int keyLength);

	int openFile(const char* name);

	int deleteFile(const char* name);

	int closeFile();

	int insert(void* key, int pid);

	int search(void* key);

	int update(void* key, int pid);

	int remove(void* key);

    void printFileDescription();

    static void test();
};

#endif
