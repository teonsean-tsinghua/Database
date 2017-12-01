#ifndef DBDATAFILE_H_INCLUDED
#define DBDATAFILE_H_INCLUDED

#include"../DBInclude.h"
#include"DBDataFileDescriptionPage.h"
#include"DBDataPage.h"
#include"DBUsagePage.h"

class DBDataFile;

class SelectResult
{
private:
    std::list<std::vector<void*> > results;

public:
    SelectResult();

    void filterByValue(std::map<int, void*>& info, int op, DBRecordInfo* ri);

    void filterByFields(std::map<int, int>& info, int op, DBRecordInfo* ri);

    void print();

    friend DBRecordSlot;
    friend DBDataFile;
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

    void remove(std::map<std::string, void*>& data);

    void update(std::map<std::string, void*>& key_value, std::map<std::string, void*>& update_value);

    void setPrimaryKey(const char* name);

    void getAllFields(std::vector<std::string>& names);

    void printRecordDescription();

    void printFileDescription();

    void printAllRecords();
};

#endif // DBDATAFILE_H_INCLUDED
