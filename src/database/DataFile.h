#ifndef DATAFILE_H_INCLUDED
#define DATAFILE_H_INCLUDED

#include"DataFileDescPage.h"
#include"FileIOModel.h"
#include"NodePage.h"
#include"UsagePage.h"
#include"IndexTree.h"
#include"DataPage.h"
#include"IndexFile.h"

class DataFile: public BaseFile
{
private:
	const static std::string TAG;
    DataFileDescPage* dfdp;
    IndexTree<PrimKey>* tree;
    std::map<int, Page*> pages;
    std::map<int, BaseFile*> indexes;
    std::map<int, bool> uniqueIndex;
    RecordInfo* ri;
    bool open;
    std::string tbname;
    std::string dbname;

    BaseFile* getIndexFile(int i);

    int findFirstAvailableDataPage();

    int allocateUsagePage();

    int allocateNodePage(bool isLeaf);

    int allocateDataPage();

    void markAsUsable(int n);

    void setRootPage(int n);

    Page* openPage(int pid);

    bool validateInsertion(std::vector<void*>& data);

    char* generatePrimKey(std::vector<void*>& data);

//    friend Table;

public:
    DataFile();

    ~DataFile();

    RecordInfo* getRecordInfo();

    void createFile(std::string dbname, std::string tbname);

    void openFile(std::string dbname, std::string tbname);

    bool insert(std::vector<void*>& data);

    void closeFile();

    void addFields(std::vector<std::string>& name, std::vector<int>& type, std::vector<int>& nullable,
    		std::vector<int>& extra, std::vector<std::string>& foreign, std::vector<bool>& isPrim);

    void createIndex(std::string fdname, bool unique = false, int density = 1);

    void printFileDesc();

    void printAllRecords();

    static void test();
};

#endif // DATAFILE_H_INCLUDED
