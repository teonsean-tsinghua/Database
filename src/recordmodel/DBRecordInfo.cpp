#include"DBRecordInfo.h"
#include"../utils/DBLog.h"
#include<cstdio>

const int DBRecordInfo::SIZEOF_TYPE[] = {10 * sizeof(char),
                                        sizeof(int),
                                        sizeof(long),
                                        sizeof(char),
                                        sizeof(char),
                                        sizeof(float),
                                        sizeof(double),
                                        256 * sizeof(char),
                                        1024 * sizeof(char)};

const std::string DBRecordInfo::NAMEOF_TYPE[] = {"RID",
                                                "int",
                                                "long",
                                                "boolean",
                                                "char",
                                                "float"
                                                "double",
                                                "string",
                                                "long string"};

DBRecordInfo::DBRecordInfo(std::vector<std::string> &n,
                           std::vector<int> &t)
{
    column_count = n.size();
    names = new std::string[column_count];
    types = new int[column_count];
    offsets = new int[column_count];
    offsets[0] = 0;
    for(int i = 0; i < n.size(); i++)
    {
        names[i] = n[i];
        types[i] = t[i];
        name_to_index[names[i]] = i;
        if(i > 0)
        {
            offsets[i] = offsets[i - 1] + SIZEOF_TYPE[types[i - 1]];
        }
        char* msg = new char[64];
        sprintf(msg, "Column %s is of type %s, at offset %d", names[i].c_str(), NAMEOF_TYPE[types[i]].c_str(), offsets[i]);
        log(std::string(msg));
    }
    length = offsets[n.size() - 1] + SIZEOF_TYPE[types[n.size() - 1]];
    char* msg = new char[64];
    sprintf(msg, "Record length is %d.", length);
    log(std::string(msg));
}
