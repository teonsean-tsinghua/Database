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
    DBFileIOModel* fm;
    DBIndexFileDescriptionPage* ifdp;
    std::map<int, DBIndexNodePage*> pages;
    int rootNode;
    int minDgr;
    int maxDgr;
    bool open;
    const char* path;

    int split(DBIndexNodePage* cur);

    int merge(DBIndexNodePage* cur);

    void _test();

public:
	DBIndexFile(const char* path);

	int allocateNewLeafNode();

	int allocateNewInternalNode();

	DBIndexNodePage* openNode(int pid);

	int createFile(int keyType, int keyLength);

	int openFile();

	int deleteFile();

	int closeFile();

	int insert(void* key, int pid);

	int search(void* key);

	int update(void* key, int pid);

	int remove(void* key);

    void printFileDescription();

    static void test();
};

#endif
