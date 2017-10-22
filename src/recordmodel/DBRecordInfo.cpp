#include"DBRecordInfo.h"
#include"../utils/DBLog.h"

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
    name_to_index = new std::map<std::string, int>();
    names = new std::string[column_count];
    types = new int[column_count];
    offsets = new int[column_count];
    offsets[0] = 0;
    for(int i = 0; i < n.size(); i++)
    {
        names[i] = n[i];
        types[i] = t[i];
        if(i > 0)
        {
            offsets[i] = offsets[i - 1] + SIZEOF_TYPE[types[i - 1]];
        }
        log("Column " + names[i] + " is of type ") + NAMEOF_TYPE[types[i]] + ", at offset " + string(offsets[i]));
    }
    length = offsets[n - 1] + SIZEOF_TYPE[types[n - 1]];
    log("Record length is " + string(length));
}
