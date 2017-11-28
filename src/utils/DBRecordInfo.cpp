#include"DBRecordInfo.h"

DBRecordInfo* DBRecordInfo::instance = new DBRecordInfo();

DBRecordInfo::DBRecordInfo()
{

}

DBRecordInfo* DBRecordInfo::getInstance()
{
    if(instance == NULL)
    {
        instance = new DBRecordInfo();
    }
    return instance;
}

int DBRecordInfo::index(std::string name)
{
    return indexes[name];
}

std::string DBRecordInfo::name(int index)
{
    return names[i];
}

bool DBRecordInfo::nullable(std::string name)
{
    return nullables[indexes[name]];
}

int DBRecordInfo::type(std::string name)
{
    return types[indexes[name]];
}

int DBRecordInfo::offset(std::string name)
{
    return offsets[indexes[name]];
}

int DBRecordInfo::extra(std::string name)
{
    return extras[indexes[name]];
}

int DBRecordInfo::length(std::string name)
{
    if(type(name) == DBType::VARCHAR)
}

void DBRecordInfo::init()
{
    indexes.clear();
    names.clear();
    types.clear();
    offsets.clear();
    nullables.clear();
}

int DBRecordInfo::getFieldCount()
{
    if(indexes.size() != names.size() ||
       names.size() != types.size() ||
       types.size() != offsets.size() ||
       offsets.size() != nullables.size())
    {
        return -1;
    }
    return indexes.size();
}
