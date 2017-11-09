#ifndef DBPARSER_H_INCLUDED
#define DBPARSER_H_INCLUDED

#include"DBFileIOModel.h"
#include<string>
#include<vector>
#include<map>
#include"DBType.h"

class DBParser
{
public:
    static void parseRecordInfo(std::map<std::string, int>& indexes,
                                std::vector<std::string>& names,
                                std::vector<int>& types,
                                std::vector<int>& offsets,
                                char* cache,
                                int length);

    static void writeRecordInfo(std::map<std::string, int>& indexes,
                                std::vector<std::string>& names,
                                std::vector<int>& types,
                                std::vector<int>& offsets,
                                char* cache,
                                int length);

    const static int RECORD_INFO_TYPE_OFFSET = 0;
    const static int RECORD_INFO_NAME_LENGTH_OFFSET = RECORD_INFO_TYPE_OFFSET + sizeof(int);
    const static int RECORD_INFO_NAME_OFFSET = RECORD_INFO_NAME_LENGTH_OFFSET + sizeof(int);
};

#endif // DBPARSER_H_INCLUDED
