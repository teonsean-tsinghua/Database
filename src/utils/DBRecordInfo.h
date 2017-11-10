#ifndef DBRECORDINFO_H_INCLUDED
#define DBRECORDINFO_H_INCLUDED

#include<map>
#include<string>
#include<vector>

class DBRecordInfo
{
public:
     std::map<std::string, int> indexes;
     std::vector<std::string> names;
     std::vector<bool> nullables;
     std::vector<int> types;
     std::vector<int> offsets;

     int getFieldCount();
};

#endif // DBRECORDINFO_H_INCLUDED
