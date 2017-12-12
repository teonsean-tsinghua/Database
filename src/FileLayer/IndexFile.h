#ifndef INDEXFILE_H_INCLUDED
#define INDEXFILE_H_INCLUDED
#include "Include.h"
#include "IndexFileDescriptionPage.h"
#include "IndexNodePage.h"
#include "UsagePage.h"

class IndexFile{
private:
    int fileID;
    int keyLength;
    int keyType;
    FileIOModel* fm;
    IndexFileDescriptionPage* ifdp;
    std::map<int, IndexNodePage*> pages;
    int rootNode;
    int minDgr;
    int maxDgr;
    bool open;
    std::string path;

    void split(IndexNodePage* cur);

    void merge(IndexNodePage* cur);

    void _test();

public:
	IndexFile(std::string path);

	int allocateNewLeafNode();

	int allocateNewInternalNode();

	IndexNodePage* openNode(int pid);

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
