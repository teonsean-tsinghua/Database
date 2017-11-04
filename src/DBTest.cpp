#include <iostream>
#include <string>
#include <cstdlib>
#include "recordmodel/DBFileManager.h"
#include "recordmodel/DBRecordInfo.h"
#include "recordmodel/DBRecordUsage.h"
#include "utils/DBLog.h"
#include "utils/printpage.h"
using namespace std;

int main()
{
    DBFileManager* manager = new DBFileManager("../data");
    manager -> createDatabase("test1");
    manager -> openDatabase("test1");
    manager -> createTable("test2");
    manager -> openTable("test2");
    std::vector<std::string> names;
    std::vector<int> types;
    std::vector<int> lengths;
    names.push_back("ID");
    types.push_back(0);
    lengths.push_back(1);
    names.push_back("Details");
    types.push_back(1);
    lengths.push_back(5);
    names.push_back("Other Infos");
    types.push_back(1);
    lengths.push_back(5);
    DBRecordInfo* recordInfoParser = new DBRecordInfo();
    recordInfoParser -> set(names, types, lengths);
    manager -> writeToDescriptionPage(recordInfoParser -> toBinary());
    std::vector<std::string> record1;
    std::vector<int> recordlen1;
    record1.push_back("axesffsd");
    record1.push_back("as12sadasdas24dsazks");
    record1.push_back("as12sadassaifuiu2sds");
    recordlen1.push_back(8);
    recordlen1.push_back(20);
    recordlen1.push_back(20);
    manager -> insertData(record1, recordlen1);
    std::vector<std::string> record2;
    record2.push_back("bxesffsd");
    record2.push_back("bs12sadasdas24dsazks");
    record2.push_back("bs12sadassaifuiu2sds");
    manager -> insertData(record2, recordlen1);
    std::vector<unsigned long long>* key = manager -> getDataByKey((unsigned char*)"Details", (unsigned char*)"as12sadasdas24dsazks", 20);
    manager -> deleteData(key);
    return 0;
}