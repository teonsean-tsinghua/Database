#ifndef DBRECORD_INFO_H_INCLUDED
#define DBRECORD_INFO_H_INCLUDED

#include<map>
#include<string>
#include<vector>

class DBRecordInfo
{
private:
    std::string* names;
    int* types;
    int* lengths;
    int* offsets;
    int column_count;
    int totalLength;

public:
    DBRecordInfo(std::vector<std::string> &_names,
                 std::vector<int> &_types,
                 std::vector<int> &_lengths);
    DBRecordInfo();
    ~DBRecordInfo();

    unsigned char* toBinary();
    void set(std::vector<std::string> &_names, std::vector<int> &_types, std::vector<int> &_lengths);
    int fromBinary(const unsigned char* binaryArray);
    int getOffsetByName(const char* name);
    int getLengthByName(const char* name);
    unsigned char* parseData(std::vector<std::string> &data, std::vector<int> &len);
    int getRecordLen();

    const static int SUCCEED = 0;
    const static int FROMBINARY_FAIL = SUCCEED + 1;
    const static int PARSE_FAIL = FROMBINARY_FAIL + 1;

    const static int TYPE_RID = 0;  // A 8 Bytes char array(can be implement to unsigned Long Long)
    const static int TYPE_INT = TYPE_RID + 1;
    const static int TYPE_LONG = TYPE_INT + 1;
    const static int TYPE_BOOL = TYPE_LONG + 1;
    const static int TYPE_CHAR = TYPE_BOOL + 1;
    const static int TYPE_FLOAT = TYPE_CHAR + 1;
    const static int TYPE_DOUBLE = TYPE_FLOAT + 1;
    // It seems that type string and long string are useless here.
    const static int TYPE_STRING = TYPE_DOUBLE + 1;
    const static int TYPE_LONG_STRING = TYPE_STRING + 1;
    const static int SIZEOF_TYPE[];
    const static std::string NAMEOF_TYPE[];
};

#endif // DBRECORD_INFO_H_INCLUDED
