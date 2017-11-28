#ifndef DBRECORDINFO_H_INCLUDED
#define DBRECORDINFO_H_INCLUDED

#include<map>
#include<string>
#include<vector>

class DBRecordInfo
{
private:
    static DBRecordInfo* instance;

    DBRecordInfo();

    std::map<std::string, int> indexes;
    std::vector<std::string> names;
    std::vector<bool> nullables;
    std::vector<int> types;
    std::vector<int> offsets;
    std::vector<int> extras;

public:
    int index(std::string name);

    std::string name(int index);

    bool nullable(std::string name);

    int type(std::string name);

    int length(std::string name);

    int offset(std::string name);

    int extra(std::string name);

    int getFieldCount();

    DBRecordInfo* getInstance();

    void init();
};

#endif // DBRECORDINFO_H_INCLUDED
