#include"DBParser.h"
#include<cstring>

void DBParser::parseRecordInfo(std::map<std::string, int>& indexes,
                            std::vector<std::string>& names,
                            std::vector<int>& types,
                            std::vector<int>& offsets,
                            char* cache,
                            int length)
{
    char* end = cache + length;
    int index = 0, offset = 0, type = 0, name_length = 0;
    while(cache < end)
    {
        sscanf(cache + RECORD_INFO_TYPE_OFFSET, "%d", &type);
        sscanf(cache + RECORD_INFO_NAME_LENGTH_OFFSET, "%d", &name_length);
        cache += RECORD_INFO_NAME_OFFSET;
        std::string name;
        name = name.assign(cache, name_length);
        indexes[name] = index;
        names.push_back(name);
        types.push_back(type);
        offsets.push_back(offset);
        offset += DBType::size(type);
        cache += name_length;
        index++;
    }
}

void DBParser::writeRecordInfo(std::map<std::string, int>& indexes,
                            std::vector<std::string>& names,
                            std::vector<int>& types,
                            std::vector<int>& offsets,
                            char* cache,
                            int length)
{

}
