#ifndef INDEXFILE_H_INCLUDED
#define INDEXFILE_H_INCLUDED
#include "Include.h"
#include "IndexFileDescPage.h"
#include "IndexNodePage.h"
#include "BucketPage.h"

class IndexFile{
private:
    int fileID;
    int keyLength;
    int keyType;
    FileIOModel* fm;
    IndexFileDescPage* ifdp;
    std::map<int, IndexNodePage*> pages;
    std::map<int, BucketPage*> buckets;
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

    int allocateNewBucketPage();

	IndexNodePage* openNode(int pid);

	void createFile(int keyType, int keyLength, bool unique);

	void openFile();

	void deleteFile();

	void closeFile();

	void insert(void* key, int pid);

	int search(void* key);

	void update(void* key, int pid);

	void remove(void* key);

    void printFileDesc();

    static void test();
};

#endif
