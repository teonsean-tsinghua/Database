#ifndef RECORDINFO_H_INCLUDED
#define RECORDINFO_H_INCLUDED

#include<map>
#include<string>
#include<vector>
#include"Exception.h"
#include"Type.h"

class RecordInfo
{
private:
	const static std::string TAG;
    std::map<std::string, int> indexes;
    std::vector<std::string> names;
    std::vector<bool> nullables;
    std::vector<int> types;
    std::vector<int> offsets;
    std::vector<int> extras;
    std::vector<std::string> foreigns;
    std::vector<int> primKeyInfo;
    int recordLength;
    int recordInfoLength;
    int primKeyCnt;
    int primKeyLen;

public:
    RecordInfo();

    void printRecordDesc(std::string tbname);

    void init();

    bool contains(std::string name);

    int index(std::string name);

    std::string name(int index);

    bool nullable(std::string name);

    int type(std::string name);

    int length(std::string name);

    int offset(std::string name);

    int extra(std::string name);

    std::string foreign(std::string name);

    bool nullable(int i);

    int type(int i);

    int length(int i);

    int displayLength(int i);

    int offset(int i);

    int extra(int i);

    std::string foreign(int i);

    int getRecordLength();

    int getRecordInfoLength();

    void setRecordInfoLength(int n);

    int getFieldCount();

    void setPrimKeyCnt(int n);

    int getPrimKeyLen();

    const std::vector<int>& getPrimKeyInfo();

    int addField(std::string name, int type, bool nullable, int extra, std::string foreign);

    void reset(int n);

    const static int SUCCEED = 0;

    const static int EMPTY_FIELD_NAME = 156;

    const static int FIELD_ALREADY_EXIST = 561;

    const static int EXCEED_PAGE_LIMIT = 615;
};

#endif // RECORDINFO_H_INCLUDED
