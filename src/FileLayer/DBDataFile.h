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
    std::string path;
    DBFileIOModel* fm;
    DBDataFileDescriptionPage* dfdp;
    std::map<int, DBPage*> pages;
    DBRecordInfo* ri;
    int lastUsagePage;
    int lastDataPage;
    bool open;

    int findFirstAvailableDataPage();

    int setAvailableOfDataPage(int dpid, bool available);

    int allocateNewDataPage();

    int allocateNewUsagePage();

    DBDataPage* openDataPage(int pid);

    DBUsagePage* openUsagePage(int pid);

    void processKeyValue(std::map<std::string, void*>& data,
                         std::map<int, void*>& processed,
                         std::vector<std::string>& errors);

    void processWriteValue(std::map<std::string, void*>& data,
                            std::vector<void*>& processed,
                            std::map<std::string, int>& errors);

    bool validateInsertion(std::vector<void*>& data);

    void _test();

public:
    DBDataFile(std::string path);

    int createFile();

    int openFile();

    int closeFile();

    int deleteFile();

    int addFields(std::vector<std::string>& name, std::vector<int>& type,
                  std::vector<bool>& nullable, std::vector<int>& extra);

    int insertRecord(std::map<std::string, void*>& fields);

    int insert(std::vector<void*>& fields);

    int findEqual(std::map<std::string, void*>& data, std::set<std::map<std::string, void*>*>& result);

    int remove(std::map<std::string, void*>& data);

    int update(std::map<std::string, void*>& key_value, std::map<std::string, void*>& update_value);

    int setPrimaryKey(const char* name);

    bool validateFields(std::vector<std::string>& names, std::string tableName);

    void getAllFields(std::vector<std::string>& names);

    void printRecordDescription();

    void printFileDescription();

    void printAllRecords();

    static void test();
};

#endif // DBDATAFILE_H_INCLUDED
