#include"DBRecordInfo.h"
#include"../DBInclude.h"
#include"../FileLayer/DBDataFileDescriptionPage.h"

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
    if(!indexes.count(name))
    {
        return -1;
    }
    return indexes[name];
}

std::string DBRecordInfo::name(int i)
{
    return names[i];
}

bool DBRecordInfo::nullable(int i)
{
    return nullables[i];
}

int DBRecordInfo::type(int i)
{
    return types[i];
}

int DBRecordInfo::offset(int i)
{
    return offsets[i];
}

int DBRecordInfo::extra(int i)
{
    return extras[i];
}

int DBRecordInfo::length(int i)
{
    if(type(i) == DBType::VARCHAR)
    {
        return extra(i);
    }
    return DBType::typeSize(type(i));
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
    {
        return extra(name);
    }
    return DBType::typeSize(type(name));
}

void DBRecordInfo::init()
{
    if(indexes.size() > 0)
    {
        indexes.clear();
    }
    names.clear();
    types.clear();
    offsets.clear();
    nullables.clear();
    extras.clear();
    recordLength = recordInfoLength = 0;
}

int DBRecordInfo::addField(std::string name, int type, bool nullable, int extra)
{
    if(name.empty())
    {
        return EMPTY_FIELD_NAME;
    }
    if(indexes.count(name) != 0)
    {
        return FIELD_ALREADY_EXIST;
    }
    if(recordInfoLength + (sizeof(int) * 4 + name.size()) > DBDataFileDescriptionPage::maxRecordInfoLength())
    {
        return EXCEED_PAGE_LIMIT;
    }
    recordInfoLength += (sizeof(int) * 4 + name.size());
    names.push_back(name);
    types.push_back(type);
    nullables.push_back(nullable);
    extras.push_back(extra);
    offsets.push_back(recordLength);
    indexes[name] = names.size() - 1;
    recordLength += length(name);
    return SUCCEED;
}

void DBRecordInfo::reset(int n)
{
    for(int i = 0; i < n; i++)
    {
        types.pop_back();
        nullables.pop_back();
        extras.pop_back();
        offsets.pop_back();
        indexes.erase(names.back());
        names.pop_back();
    }
}

int DBRecordInfo::getRecordLength()
{
    return recordLength;
}

int DBRecordInfo::getRecordInfoLength()
{
    return recordInfoLength;
}

void DBRecordInfo::setRecordInfoLength(int n)
{
    recordInfoLength = n;
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
