#ifndef DBRECORD_INFO_H_INCLUDED
#define DBRECORD_INFO_H_INCLUDED

#include<map>
#include<string>
#include<vector>

class DBRecordInfo
{
private:
    std::map<std::string, int> name_to_index;
    std::string* names;
    int* types;
    int* offsets;
    int column_count;
    int length;

    const static int TYPE_RID = 0;  // A 10 Bytes char array
    const static int TYPE_INT = TYPE_RID + 1;
    const static int TYPE_LONG = TYPE_INT + 1;
    const static int TYPE_BOOL = TYPE_LONG + 1;
    const static int TYPE_CHAR = TYPE_BOOL + 1;
    const static int TYPE_FLOAT = TYPE_CHAR + 1;
    const static int TYPE_DOUBLE = TYPE_FLOAT + 1;
    const static int TYPE_STRING = TYPE_DOUBLE + 1;
    const static int TYPE_LONG_STRING = TYPE_STRING + 1;
    const static int SIZEOF_TYPE[];
    const static std::string NAMEOF_TYPE[];

public:
    DBRecordInfo(std::vector<std::string> &names,
                 std::vector<int> &types);
};

#endif // DBRECORD_INFO_H_INCLUDED
