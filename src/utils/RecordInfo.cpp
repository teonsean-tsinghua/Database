#include"RecordInfo.h"
#include"../Include.h"
#include"../FileLayer/DataFileDescriptionPage.h"

SearchInfo::SearchInfo()
{
    nulls.clear();
    for(int i = 0; i < 6; i++)
    {
        values[i].clear();
        fields[i].clear();
    }
}

RecordInfo::RecordInfo()
{
    indexes.clear();
    names.clear();
    types.clear();
    offsets.clear();
    nullables.clear();
    extras.clear();
    recordLength = recordInfoLength = 0;
}

void RecordInfo::init()
{
    indexes.clear();
    names.clear();
    types.clear();
    offsets.clear();
    nullables.clear();
    extras.clear();
    recordLength = recordInfoLength = 0;
}

int RecordInfo::index(std::string name)
{
    if(!indexes.count(name))
    {
        return -1;
    }
    return indexes[name];
}

std::string RecordInfo::name(int i)
{
    return names[i];
}

bool RecordInfo::nullable(int i)
{
    return nullables[i];
}

int RecordInfo::type(int i)
{
    return types[i];
}

int RecordInfo::offset(int i)
{
    return offsets[i];
}

int RecordInfo::extra(int i)
{
    return extras[i];
}

int RecordInfo::length(int i)
{
    if(type(i) == Type::VARCHAR)
    {
        return extra(i);
    }
    return Type::typeSize(type(i));
}

bool RecordInfo::nullable(std::string name)
{
    return nullables[indexes[name]];
}

int RecordInfo::type(std::string name)
{
    return types[indexes[name]];
}

int RecordInfo::offset(std::string name)
{
    return offsets[indexes[name]];
}

int RecordInfo::extra(std::string name)
{
    return extras[indexes[name]];
}

int RecordInfo::length(std::string name)
{
    if(type(name) == Type::VARCHAR)
    {
        return extra(name);
    }
    return Type::typeSize(type(name));
}

bool RecordInfo::contains(std::string name)
{
    return indexes.count(name);
}

int RecordInfo::addField(std::string name, int type, bool nullable, int extra)
{
    if(name.empty())
    {
        return EMPTY_FIELD_NAME;
    }
    if(indexes.count(name) != 0)
    {
        return FIELD_ALREADY_EXIST;
    }
    if(recordInfoLength + (sizeof(int) * 4 + name.size()) > DataFileDescriptionPage::maxRecordInfoLength())
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
    recordLength += (1 + length(name));
    return SUCCEED;
}

void RecordInfo::reset(int n)
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

int RecordInfo::getRecordLength()
{
    return recordLength;
}

int RecordInfo::getRecordInfoLength()
{
    return recordInfoLength;
}

void RecordInfo::setRecordInfoLength(int n)
{
    recordInfoLength = n;
}

int RecordInfo::getFieldCount()
{
    if(indexes.size() != names.size() ||
       names.size() != types.size() ||
       types.size() != offsets.size() ||
       offsets.size() != nullables.size())
    {
        throw Exception("Invalid record info structure.");
    }
    return indexes.size();
}
