#ifndef DBRECORD_INFO_H_INCLUDED
#define DBRECORD_INFO_H_INCLUDED

#include<hash_map>

class DBRecordInfo
{
public:
    std::hash_map<std::string, int> name_to_index;
    std::vector<std::string> column_name;
    std::vector<int> column_type;
    std::vector<int> column_offset;
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
    const static int SIZEOF_TYPE[] = {10 * sizeof(char),
                                      sizeof(int),
                                      sizeof(char),
                                      sizeof(char),
                                      sizeof(float),
                                      sizeof(double),
                                      256 * sizeof(char),
                                      1024 * sizeof(char)};
};

#endif // DBRECORD_INFO_H_INCLUDED
