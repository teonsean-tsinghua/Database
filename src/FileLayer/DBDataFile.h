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
    DBPage* visit(int pageID);

    int findFirstAvailableDataPage();

    int setAvailableOfDataPage(int dpid, bool available);

    int allocateNewDataPage();

    int allocateNewUsagePage();

    DBDataPage* openDataPage(int pid);

    DBUsagePage* openUsagePage(int pid);

    int insertRecordToPage(int page, std::vector<void*>& processed);

public:
    DBDataFile(const char* root);

    int createFile(const char* name);

    int openFile(const char* name);

    int closeFile();

    int deleteFile(const char* name);

    int addField(const char* name, int type, bool nullable);

    //If a nullable field is set to null, then don't include it in fields.
    int insertRecord(std::map<std::string, void*>& fields);

    int setPrimaryKey(const char* name);

    void printFileDescription();
};

#endif // DBDATAFILE_H_INCLUDED
