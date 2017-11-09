#include"DBParser.h"

int DBParser::parseRecordInfo(std::map<std::string, int>& indexes,
                            std::vector<std::string>& names,
                            std::vector<int>& types,
                            std::vector<int>& offsets,
                            char* cache,
                            int length)
{
    char* end = cache + length;
    int index = 0, offset = 0, type = 0, name_length = 0;
    if(!indexes.empty() ||
       !names.empty() ||
       !types.empty() ||
       !offsets.empty())
    {
        return ERROR;
    }
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
        offset += DBType::typeSize(type);
        cache += name_length;
        index++;
    }
    return SUCCEED;
}

int DBParser::writeRecordInfo(std::vector<std::string>& names,
                              std::vector<int>& types,
                              char* cache,
                              int& length)
{
    length = 0;
    if(names.size() != types.size())
    {
        return ERROR;
    }
    for(int i = 0; i < names.size(); i++)
    {
        int name_length = names[i].size();
        sprintf(cache + RECORD_INFO_TYPE_OFFSET, "%d", types[i]);
        sprintf(cache + RECORD_INFO_NAME_LENGTH_OFFSET, "%d", name_length);
        length += 2 * sizeof(int);
        cache += RECORD_INFO_NAME_OFFSET;
        if(names[i].copy(cache, name_length) != name_length)
        {
            return ERROR;
        }
        length += name_length;
        cache += name_length;
    }
    return SUCCEED;
}
