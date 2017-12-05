#ifndef DBINDEXFILE_H_INCLUDED
#define DBINDEXFILE_H_INCLUDED
#include "../DBInclude.h"
#include "DBIndexFileDescriptionPage.h"
#include "DBIndexNodePage.h"
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
    std::string path;

    void split(DBIndexNodePage* cur);

    void merge(DBIndexNodePage* cur);

    void _test();

public:
	DBIndexFile(std::string path);

	int allocateNewLeafNode();

	int allocateNewInternalNode();

	DBIndexNodePage* openNode(int pid);

	void createFile(int keyType, int keyLength);

	void openFile();

	void deleteFile();

	void closeFile();

	void insert(void* key, int pid);

	int search(void* key);

	void update(void* key, int pid);

	void remove(void* key);

    void printFileDescription();

    static void test();
};

#endif
