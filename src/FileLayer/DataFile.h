#ifndef DATAFILE_H_INCLUDED
#define DATAFILE_H_INCLUDED

#include"Include.h"
#include"DataFileDescriptionPage.h"
#include"DataPage.h"
#include"UsagePage.h"

class DataFile;

class SelectResult
{

public:
    SelectResult();

    std::list<std::vector<void*> > results;

    void filterByValue(std::map<int, std::vector<void*> >& info, int op, RecordInfo* ri);

    void filterByFields(std::map<int, std::vector<int> >& info, int op, RecordInfo* ri);

    void print();
};

class DataFile
{
private:
    int fileID;
    std::string path;
    FileIOModel* fm;
    DataFileDescriptionPage* dfdp;
    std::map<int, Page*> pages;
    RecordInfo* ri;
    int lastUsagePage;
    int lastDataPage;
    bool open;

    int findFirstAvailableDataPage();

    void setAvailableOfDataPage(int dpid, bool available);

    int allocateNewDataPage();

    int allocateNewUsagePage();

    DataPage* openDataPage(int pid);

    UsagePage* openUsagePage(int pid);

    bool validateInsertion(std::vector<void*>& data);

public:
    DataFile(std::string path);

    RecordInfo* getRecordInfo();

    void createFile();

    void openFile();

    void closeFile();

    void deleteFile();

    void addFields(std::vector<std::string>& name, std::vector<int>& type,
                  std::vector<bool>& nullable, std::vector<int>& extra);

    void insert(std::vector<void*>& fields);

    void select(SearchInfo& si, SelectResult& sr);

    int remove(SearchInfo& si);

    int update(SearchInfo& si, UpdateInfo& ui);

    void setPrimaryKey(const char* name);

    void getAllFields(std::vector<std::string>& names);

    void printRecordDescription();

    void printFileDescription();

    void printAllRecords();
};

#endif // DATAFILE_H_INCLUDED
