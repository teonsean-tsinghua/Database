#ifndef DBDATAFILE_H_INCLUDED
#define DBDATAFILE_H_INCLUDED

#include"../DBInclude.h"
#include"DBDataFileDescriptionPage.h"
#include"DBDataPage.h"
#include"DBUsagePage.h"

class DBDataFile;

class SelectResult
{

public:
    SelectResult();

    std::list<std::vector<void*> > results;

    void filterByValue(std::map<int, std::vector<void*> >& info, int op, DBRecordInfo* ri);

    void filterByFields(std::map<int, std::vector<int> >& info, int op, DBRecordInfo* ri);

    void print();
};

class DBDataFile
{
private:
    int fileID;
    std::string path;
    DBFileIOModel* fm;
    DBDataFileDescriptionPage* dfdp;
    std::map<int, DBPage*> pages;
    DBRecordInfo* ri;
    int lastUsagePage;
    int lastDataPage;
    bool open;

    int findFirstAvailableDataPage();

    void setAvailableOfDataPage(int dpid, bool available);

    int allocateNewDataPage();

    int allocateNewUsagePage();

    DBDataPage* openDataPage(int pid);

    DBUsagePage* openUsagePage(int pid);

    bool validateInsertion(std::vector<void*>& data);

public:
    DBDataFile(std::string path);

    DBRecordInfo* getRecordInfo();

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

#endif // DBDATAFILE_H_INCLUDED
