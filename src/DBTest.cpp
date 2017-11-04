#include <iostream>
#include <string>
#include <cstdlib>
#include "recordmodel/DBFileManager.h"
#include "recordmodel/DBRecordInfo.h"
#include "utils/DBLog.h"
#include "utils/printpage.h"
using namespace std;

int main()
{
    std::vector<std::string> _names;
    std::vector<int> _types;
    std::vector<int> _lengths;
    _names.push_back("test1");
    _types.push_back(0);
    _lengths.push_back(35);
    _names.push_back("test2");
    _types.push_back(4);
    _lengths.push_back(12);
    DBRecordInfo* rif = new DBRecordInfo(_names, _types, _lengths);
    unsigned char* bindetail = rif -> toBinary();
    DBFileManager* manager = new DBFileManager("../data");
    manager -> createDatabase("test1");
    manager -> openDatabase("test1");
    manager -> createTable("test2");
    manager -> openTable("test2");
    manager -> writeToDescriptionPage(bindetail);
    return 0;
}