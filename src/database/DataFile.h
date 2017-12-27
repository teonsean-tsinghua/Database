#ifndef DATAFILE_H_INCLUDED
#define DATAFILE_H_INCLUDED

#include"DataFileDescPage.h"
#include"FileIOModel.h"

class DataFile
{
private:
	const static std::string TAG;
    int fileID;
    FileIOModel* fm;
    DataFileDescPage* dfdp;
    std::map<int, Page*> pages;
    RecordInfo* ri;
    int lastUsagePage;
    int lastDataPage;
    bool open;

    int findFirstAvailableDataPage();

    void setAvailableOfDataPage(int dpid, bool available);

    int allocateNewDataPage();

    int allocateNewUsagePage();

//    DataPage* openDataPage(int pid);
//
//    UsagePage* openUsagePage(int pid);

    bool validateInsertion(std::vector<void*>& data);

//    friend Table;

public:
    DataFile();

    ~DataFile();

    RecordInfo* getRecordInfo();

    void createFile(std::string dbname, std::string tbname);

    void openFile(std::string dbname, std::string tbname);

    void closeFile();

    void addFields(std::vector<std::string>& name, std::vector<int>& type, std::vector<int>& nullable,
    		std::vector<int>& extra, std::vector<std::string>& foreign, int primCnt);

//    void createIndex(IndexFile& inf);

//    void insert(std::vector<void*>& fields);
//
//    void select(SearchInfo& si, SelectResult& sr);
//
//    int remove(SearchInfo& si);
//
//    int update(SearchInfo& si, UpdateInfo& ui);

    void printFileDesc();

    void printAllRecords();

    static void test();
};

#endif // DATAFILE_H_INCLUDED
