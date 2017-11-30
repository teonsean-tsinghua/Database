#ifndef DBRECORDINFO_H_INCLUDED
#define DBRECORDINFO_H_INCLUDED

#include<map>
#include<string>
#include<vector>

class DBRecordInfo
{
private:
    std::map<std::string, int> indexes;
    std::vector<std::string> names;
    std::vector<bool> nullables;
    std::vector<int> types;
    std::vector<int> offsets;
    std::vector<int> extras;
    int recordLength;
    int recordInfoLength;

public:
    DBRecordInfo();

    void init();

    bool contains(std::string name);

    int index(std::string name);

    std::string name(int index);

    bool nullable(std::string name);

    int type(std::string name);

    int length(std::string name);

    int offset(std::string name);

    int extra(std::string name);

    bool nullable(int i);

    int type(int i);

    int length(int i);

    int offset(int i);

    int extra(int i);

    int getRecordLength();

    int getRecordInfoLength();

    void setRecordInfoLength(int n);

    int getFieldCount();

    int addField(std::string name, int type, bool nullable, int extra);

    void reset(int n);
};

#endif // DBRECORDINFO_H_INCLUDED
