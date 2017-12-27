#include"Info.h"
#include"../database/DataFileDescPage.h"

const std::string RecordInfo::TAG = "RecordInfo";

RecordInfo::RecordInfo()
{
    init();
}

void RecordInfo::setPrimKeyCnt(int n)
{
	primKeyCnt = n;
	primKeyLen = 0;
	primKeyInfo.clear();
	for(int i = 0; i < n; i++)
	{
		primKeyLen += length(i);
		primKeyInfo.push_back(type(i));
		primKeyInfo.push_back(length(i));
	}
}

int RecordInfo::getPrimKeyLen()
{
	return primKeyLen;
}

const std::vector<int>& RecordInfo::getPrimKeyInfo()
{
	return primKeyInfo;
}

void RecordInfo::printRecordDesc(std::string tbname)
{
	std::cout << "TABLE " << tbname << "(\n";
    int cnt = getFieldCount();
    for(int i = 0; i < cnt; i++)
    {
        std::cout << "    " << name(i) << " " << Type::typeName(type(i));
        if(extra(i) > 0)
        {
            std::cout << "(" << extra(i) << ")";
        }
        if(!nullable(i))
        {
            std::cout << " NOT NULL";
        }
        std::cout << ",\n";
    }
    std::cout << "    PRIMARY KEY:(";
    for(int i = 0; i < primKeyCnt; i++)
    {
    	std::cout << name(i);
    	if(i != primKeyCnt - 1)
    	{
    		std::cout << ", ";
    	}
    }
    for(int i = 0; i < cnt; i++)
    {
    	if(foreign(i).size() > 0)
    	{
    		std::cout << "),\n";
    		int dot = foreign(i).find('.');
    		assert(dot > 0);
    		std::cout << "    FOREIGN KEY(" << name(i) << ") REFERENCES ";
    		std::cout << foreign(i).substr(0, dot) << " (" << foreign(i).substr(dot + 1, foreign(i).size());
    	}
    }
    std::cout << ")\n)\n";
}

void RecordInfo::init()
{
    indexes.clear();
    names.clear();
    types.clear();
    offsets.clear();
    nullables.clear();
    extras.clear();
    foreigns.clear();
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

std::string RecordInfo::foreign(int i)
{
	return foreigns[i];
}

int RecordInfo::length(int i)
{
    if(type(i) == Type::VARCHAR)
    {
        return extra(i);
    }
    return Type::typeSize(type(i));
}

int RecordInfo::displayLength(int i)
{
    switch(type(i))
    {
    case Type::INT:
        return 10;
    case Type::VARCHAR:
        return extra(i);
    }
    return 0;
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

std::string RecordInfo::foreign(std::string name)
{
	return foreigns[indexes[name]];
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

int RecordInfo::addField(std::string name, int type, bool nullable, int extra, std::string foreign)
{
    if(name.empty())
    {
        return EMPTY_FIELD_NAME;
    }
    if(indexes.count(name) != 0)
    {
        return FIELD_ALREADY_EXIST;
    }
    if(recordInfoLength + (sizeof(int) * 5 + name.size() + foreign.size()) > DataFileDescPage::maxRecordInfoLength())
    {
        return EXCEED_PAGE_LIMIT;
    }
    recordInfoLength += (sizeof(int) * 5 + name.size() + foreign.size());
    if(type == Type::VARCHAR)
    {
        extra++;
    }
    names.push_back(name);
    types.push_back(type);
    nullables.push_back(nullable);
    extras.push_back(extra);
    offsets.push_back(recordLength);
    foreigns.push_back(foreign);
    indexes[name] = names.size() - 1;
    recordLength += (1 + length(name));
    return SUCCEED;
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
       offsets.size() != nullables.size() ||
	   nullables.size() != foreigns.size())
    {
        throw Exception(TAG, "Invalid record info structure.");
    }
    return indexes.size();
}
