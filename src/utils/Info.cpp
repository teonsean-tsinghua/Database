#include"Info.h"
#include"../database/DataFileDescPage.h"
#include"Compare.h"

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

SearchInfo::SearchInfo()
{
    nulls.clear();
    for(int i = 0; i < 6; i++)
    {
        values[i].clear();
        fields[i].clear();
    }
}

bool SearchInfo::processWheresWithOneTable(std::vector<Where>& pWheres, RecordInfo* ri, std::string tbname)
{
    bool flag = true;
    for(int i = 0; i < pWheres.size(); i++)
    {
        Where& w = pWheres[i];
        if(w.left.table != "" &&
           w.left.table != tbname)
        {
            std::cout << "Table " << w.left.table << " is not selected.\n";
            flag = false;
        }
        else if(ri->index(w.left.field) < 0)
        {
            std::cout << "Table " << tbname << " does not have field" << w.left.field << ".\n";
            flag = false;
        }
        if(w.type == 2 && w.opCol)
        {
            if(w.col_r.table != "" && w.col_r.table != tbname)
            {
                std::cout << "Table " << w.col_r.table << " is not selected.\n";
                flag = false;
            }
            else if(ri->index(w.col_r.field) < 0)
            {
                std::cout << "Table " << tbname << " does not have field" << w.col_r.field << ".\n";
                flag = false;
            }
        }
    }
    if(!flag)
    {
        return false;
    }
    for(int i = 0; i < pWheres.size(); i++)
    {
        Where& w = pWheres[i];
        int lindex = ri->index(w.left.field);
        switch(w.type)
        {
        case 0:
            if(nulls.count(lindex))
            {
                if(!nulls[lindex])
                {
                    return false;
                }
            }
            nulls[lindex] = true;
            break;
        case 1:
            if(nulls.count(lindex))
            {
                if(nulls[lindex])
                {
                    return false;
                }
            }
            nulls[lindex] = false;
            break;
        case 2:
            if(w.opCol)
            {
                if(!fields[w.op].count(lindex))
                {
                    fields[w.op][lindex] = std::vector<int>();
                }
                fields[w.op][lindex].push_back(ri->index(w.col_r.field));
            }
            else
            {
                void* dat = (w.val_r.type == 1 ? (void*)&w.val_r.v_int : (void*)&w.val_r.v_str);
                if(w.op == 5)
                {
                    if(!notEqual.count(lindex))
                    {
                        notEqual[lindex] = std::vector<void*>();
                    }
                    notEqual[lindex].push_back(dat);
                }
                else if(!values[w.op].count(lindex))
                {
                    values[w.op][lindex] = dat;
                }
                else
                {
                    switch(w.op)
                    {
                    case 0:
                        if(!Equal(dat, values[0][lindex], ri->type(lindex)))
                        {
                            return false;
                        }
                        break;
                    case 1:
                        if(smaller(dat, values[1][lindex], ri->type(lindex)))
                        {
                            values[1][lindex] = dat;
                        }
                        break;
                    case 2:
                        if(larger(dat, values[1][lindex], ri->type(lindex)))
                        {
                            values[2][lindex] = dat;
                        }
                        break;
                    case 3:
                        if(smaller(dat, values[3][lindex], ri->type(lindex)))
                        {
                            values[3][lindex] = dat;
                        }
                        break;
                    case 4:
                        if(larger(dat, values[4][lindex], ri->type(lindex)))
                        {
                            values[4][lindex] = dat;
                        }
                        break;
                    }
                }
            }
        }
    }
    for(std::map<int, bool>::iterator iter = nulls.begin(); iter != nulls.end(); iter++)
    {
        int idx = iter->first;
        if((bool)(iter->second))
        {
            for(int i = 0; i < 5; i++)
            {
                if(values[i].count(idx))
                {
                    return false;
                }
            }
            notEqual.erase(idx);
        }
    }
    std::map<int, void*>::iterator it;
    for(it = values[0].begin(); it != values[0].end(); it++)
    {
        int idx = it->first;
        void* dat = it->second;
        if(values[1].count(idx) && !smallerOrEqual(dat, values[1][idx], ri->type(idx)))
        {
            return false;
        }
        if(values[2].count(idx) && !largerOrEqual(dat, values[2][idx], ri->type(idx)))
        {
            return false;
        }
        if(values[3].count(idx) && !smaller(dat, values[3][idx], ri->type(idx)))
        {
            return false;
        }
        if(values[4].count(idx) && !larger(dat, values[4][idx], ri->type(idx)))
        {
            return false;
        }
        for(int i = 1; i < 5; i++)
        {
            values[i].erase(idx);
        }
        nulls.erase(idx);
    }
    for(it = values[1].begin(); it != values[1].end();)
    {
        int idx = it->first;
        void* dat = it->second;
        if(values[2].count(idx))
        {
            if(Equal(dat, values[2][idx], ri->type(idx)))
            {
                if(smaller(dat, values[3][idx], ri->type(idx)) &&
                   larger(dat, values[4][idx], ri->type(idx)))
                {
                    values[0][idx] = dat;
                    for(int i = 2; i < 5; i++)
                    {
                        values[i].erase(idx);
                    }
                    values[1].erase(it++);
                    continue;
                }
                else
                {
                    return false;
                }
            }
            else if(smaller(dat, values[2][idx], ri->type(idx)))
            {
                return false;
            }
        }
        if(values[3].count(idx))
        {
            if(largerOrEqual(dat, values[3][idx], ri->type(idx)))
            {
                values[1].erase(it++);
                continue;
            }
            else
            {
                values[3].erase(idx);
            }
        }
        if(values[4].count(idx))
        {
            if(!larger(dat, values[4][idx], ri->type(idx)))
            {
                return false;
            }
        }
        it++;
        nulls.erase(idx);
    }
    for(it = values[2].begin(); it != values[2].end();)
    {
        int idx = it->first;
        void* dat = it->second;
        if(values[3].count(idx))
        {
            if(!smaller(dat, values[3][idx], ri->type(idx)))
            {
                return false;
            }
        }
        if(values[4].count(idx))
        {
            if(smallerOrEqual(dat, values[4][idx], ri->type(idx)))
            {
                values[2].erase(it++);
                continue;
            }
            else
            {
                values[4].erase(idx);
            }
        }
        nulls.erase(idx);
        it++;
    }
    for(it = values[3].begin(); it != values[3].end(); it++)
    {
        int idx = it->first;
        void* dat = it->second;
        if(values[4].count(idx))
        {
            if(!larger(dat, values[4][idx], ri->type(idx)))
            {
                return false;
            }
        }
        nulls.erase(idx);
    }
    for(it = values[4].begin(); it != values[4].end(); it++)
    {
        int idx = it->first;
        void* dat = it->second;
        nulls.erase(idx);
    }
    for(std::map<int, std::vector<void*> >::iterator ite = notEqual.begin(); ite != notEqual.end();)
    {
        int idx = ite->first;
        std::vector<void*>& vec = ite->second;
        if(!values[0].count(idx))
        {
            ite++;
            continue;
        }
        for(int i = 0; i < vec.size(); i++)
        {
            if(Equal(vec[i], values[0][idx], ri->type(idx)))
            {
                return false;
            }
        }
        notEqual.erase(ite++);
    }
    return true;
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
    case Type::_ID:
        return 32;
    case Type::INT:
        return 10;
    case Type::VARCHAR:
        return extra(i);
    case Type::DATE:
        return 8;
    case Type::FLOAT:
        return 10;
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
