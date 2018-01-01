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
    pthread_mutex_t page_lock;
    std::map<int, BaseFile*> indexes;
    std::map<int, bool> uniqueIndex;
    std::queue<int> candidate_pids;
    pthread_mutex_t candidate_lock;
    std::queue<int> search_result;
    pthread_mutex_t search_result_lock;
    SearchInfo* cur_search_info;
    std::vector<bool>* cur_selected;
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

    bool validate(int rid);

    void print(int rid);

    void remove(int rid);

    void update(int rid, UpdateInfo& ui, bool primChanged, std::set<int>& indexChanged);

    void terminateSearch(pthread_t* p, pthread_t* v);

    bool validateInsertion(std::vector<void*>& data);

    char* generatePrimKey(std::vector<void*>& data);

public:
    DataFile();

    ~DataFile();

    RecordInfo* getRecordInfo();

    void search(SearchInfo& si, pthread_t* workingThread);

    void remove(SearchInfo& si);

    void update(SearchInfo& si, UpdateInfo& ui);

    void createFile(std::string dbname, std::string tbname);

    void openFile(std::string dbname, std::string tbname);

    bool insert(std::vector<void*>& data);

    void select(SearchInfo& si, std::vector<bool>& selected);

    void closeFile();

    void addFields(std::vector<std::string>& name, std::vector<int>& type, std::vector<int>& nullable,
    		std::vector<int>& extra, std::vector<std::string>& foreign, std::vector<bool>& isPrim);

    void createIndex(std::string fdname, bool unique, int density);

    void printFileDesc();

    void printAllRecords();

    static void test();

    friend void* printLoop(void* df);

    friend void* validateLoop(void* df);
};

#endif // DATAFILE_H_INCLUDED
