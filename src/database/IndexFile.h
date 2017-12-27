#ifndef INDEXFILE_H_INCLUDED
#define INDEXFILE_H_INCLUDED

#include "IndexFileDescPage.h"
#include "NodePage.h"
#include "UsagePage.h"

class IndexFile
{
private:
    int fileID;
    int keyLength;
    int keyType;
    int lastUsagePage;
    int lastBucketPage;
    FileIOModel* fm;
    IndexFileDescPage* ifdp;
    std::map<int, Page*> pages;
//    int minDgr;
//    int maxDgr;
    bool open;

//    void split(IndexNodePage* cur);
//
//    void merge(IndexNodePage* cur);
//
//    void _test();
//
public:
	IndexFile(std::string path);

//	int allocateNewLeafNode();
//
//	int allocateNewInternalNode();

    InternalPage* openInternalPage(int pid);

    LeafPage* openLeafPage(int pid);

    UsagePage* openUsagePage(int pid);

    NodePage* openNodePage(int pid);

    BucketPage* openBucketPage(int pid);

	Page* openPage(int pid);

	void createFile(std::string dbname, std::string tbname, std::string fdname, int keyType, int keyLength, bool unique, int density = 1);

	void openFile(std::string dbname, std::string tbname, std::string fdname);

	void closeFile();

//	void insert(void* key, int pid);
//
//	int search(void* key);
//
//	void update(void* key, int pid);
//
//	void remove(void* key);
//
    void printFileDesc();

//    static void test();
//};

#endif
