#ifndef DBDATAFILE_H_INCLUDED
#define DBDATAFILE_H_INCLUDED

#include"../DBInclude.h"
#include"DBDataFileDescriptionPage.h"
#include"DBDataPage.h"
#include"DBUsagePage.h"

class DBDataFile
{
private:
    int fileID;
    const char* root;
    DBFileIOModel* fm;
    DBDataFileDescriptionPage* dfdp;
    std::map<int, DBPage*> pages;
    DBRecordInfo* ri;
    int lastUsagePage;
    int lastDataPage;
    const char* name;
    bool open;

    int findFirstAvailableDataPage();

    int setAvailableOfDataPage(int dpid, bool available);

    int allocateNewDataPage();

    int allocateNewUsagePage();

    DBDataPage* openDataPage(int pid);

    DBUsagePage* openUsagePage(int pid);

    int insertRecordToPage(int page, std::vector<void*>& processed);

    void processKeyValue(std::map<std::string, void*>& data,
                         std::map<int, void*>& processed,
                         std::vector<std::string>& errors);

    void processWriteValue(std::map<std::string, void*>& data,
                            std::vector<void*>& processed,
                            std::map<std::string, int>& errors);

public:
    DBDataFile(const char* root);

    int createFile(const char* name);

    int openFile(const char* name);

    int closeFile();

    int deleteFile(const char* name);

    int addField(const char* name, int type, bool nullable);

    //If a nullable field is set to null, then don't include it in fields.
    int insertRecord(std::map<std::string, void*>& fields);

    int findEqual(std::map<std::string, void*>& data, std::set<std::map<std::string, void*>*>& result);

    int remove(std::map<std::string, void*>& data);

    int update(std::map<std::string, void*>& key_value, std::map<std::string, void*>& update_value);

    int setPrimaryKey(const char* name);

    void printFileDescription();

    void printAllRecords();
};

#endif // DBDATAFILE_H_INCLUDED
