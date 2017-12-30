#include"Info.h"
#include"../database/DataFileDescPage.h"

const std::string RecordInfo::TAG = "RecordInfo";

RecordInfo::RecordInfo()
{
    init();
}

int RecordInfo::getPrimKeyLen()
{
	return primKeyLen;
}

int RecordInfo::getPrimKeyCnt()
{
    return primKeyCnt;
}

void RecordInfo::setPrimKeyCnt(int n)
{
    primKeyCnt = n;
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
    int j = 0;
    for(int i = 0; i < names.size(); i++)
    {
        if(!isPrim(i))
        {
            continue;
        }
    	std::cout << name(i);
    	j++;
    	if(j != primKeyCnt)
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
    isPrims.clear();
    primKeyInfo.clear();
    primKeyCnt = primKeyLen = recordLength = recordInfoLength = 0;
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

bool RecordInfo::isPrim(int i)
{
    return isPrims[i];
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
        return extra(i) + 1;
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

bool RecordInfo::isPrim(std::string name)
{
    return isPrims[indexes[name]];
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
        return extra(name) + 1;
    }
    return Type::typeSize(type(name));
}

bool RecordInfo::contains(std::string name)
{
    return indexes.count(name);
}

int RecordInfo::addField(std::string name, int type, bool nullable, int extra, std::string foreign, bool isPrim)
{
    if(name.empty())
    {
        return EMPTY_FIELD_NAME;
    }
    if(indexes.count(name) != 0)
    {
        return FIELD_ALREADY_EXIST;
    }
    if(recordInfoLength + (sizeof(int) * 6 + name.size() + foreign.size()) > DataFileDescPage::maxRecordInfoLength())
    {
        return EXCEED_PAGE_LIMIT;
    }
    recordInfoLength += (sizeof(int) * 6 + name.size() + foreign.size());
    names.push_back(name);
    types.push_back(type);
    nullables.push_back(nullable);
    extras.push_back(extra);
    offsets.push_back(recordLength - offsets.size());
    foreigns.push_back(foreign);
    indexes[name] = names.size() - 1;
    isPrims.push_back(isPrim);
    recordLength += (1 + length(name));
    if(isPrim)
    {
        primKeyInfo.push_back(type);
        primKeyInfo.push_back(length(names.size() - 1));
        primKeyLen += length(names.size() - 1);
        primKeyCnt++;
    }
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
//    if(indexes.size() != names.size() ||
//       names.size() != types.size() ||
//       types.size() != offsets.size() ||
//       offsets.size() != nullables.size() ||
//	   nullables.size() != foreigns.size())
//    {
//        throw Exception(TAG, "Invalid record info structure.");
//    }
    return indexes.size();
}
