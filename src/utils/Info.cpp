#include"Info.h"
#include"../FileLayer/DataFileDescPage.h"

SelectResult::SelectResult()
{
    all = false;
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
            Print::printLine("Table " + w.left.table + " is not selected.");
            flag = false;
        }
        else if(ri->index(w.left.field) < 0)
        {
            Print::printLine("Table " + tbname + " does not have field" + w.left.field + ".");
            flag = false;
        }
        if(w.type == 2 && w.opCol)
        {
            if(w.col_r.table != "" && w.col_r.table != tbname)
            {
                Print::printLine("Table " + w.col_r.table + " is not selected.");
                flag = false;
            }
            else if(ri->index(w.col_r.field) < 0)
            {
                Print::printLine("Table " + tbname + " does not have field" + w.col_r.field + ".");
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
                    pWheres.clear();
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
                    pWheres.clear();
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
                        if(!Equal(dat, values[0][lindex], ri->type(lindex), ri->length(lindex)))
                        {
                            return false;
                        }
                        break;
                    case 1:
                        if(smaller(dat, values[1][lindex], ri->type(lindex), ri->length(lindex)))
                        {
                            values[1][lindex] = dat;
                        }
                        break;
                    case 2:
                        if(larger(dat, values[1][lindex], ri->type(lindex), ri->length(lindex)))
                        {
                            values[2][lindex] = dat;
                        }
                        break;
                    case 3:
                        if(smaller(dat, values[3][lindex], ri->type(lindex), ri->length(lindex)))
                        {
                            values[3][lindex] = dat;
                        }
                        break;
                    case 4:
                        if(larger(dat, values[4][lindex], ri->type(lindex), ri->length(lindex)))
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
        if(values[1].count(idx) && !smallerOrEqual(dat, values[1][idx], ri->type(idx), ri->length(idx)))
        {
            return false;
        }
        if(values[2].count(idx) && !largerOrEqual(dat, values[2][idx], ri->type(idx), ri->length(idx)))
        {
            return false;
        }
        if(values[3].count(idx) && !smaller(dat, values[3][idx], ri->type(idx), ri->length(idx)))
        {
            return false;
        }
        if(values[4].count(idx) && !larger(dat, values[4][idx], ri->type(idx), ri->length(idx)))
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
            switch(compare(dat, values[2][idx], ri->type(idx), ri->length(idx)))
            {
            case EQUAL:
                if(smaller(dat, values[3][idx], ri->type(idx), ri->length(idx)) &&
                   larger(dat, values[4][idx], ri->type(idx), ri->length(idx)))
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
                break;
            case SMALLER:
                return false;
            }
        }
        if(values[3].count(idx))
        {
            if(largerOrEqual(dat, values[3][idx], ri->type(idx), ri->length(idx)))
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
            if(!larger(dat, values[4][idx], ri->type(idx), ri->length(idx)))
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
            if(!smaller(dat, values[3][idx], ri->type(idx), ri->length(idx)))
            {
                return false;
            }
        }
        if(values[4].count(idx))
        {
            if(smallerOrEqual(dat, values[4][idx], ri->type(idx), ri->length(idx)))
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
            if(!larger(dat, values[4][idx], ri->type(idx), ri->length(idx)))
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
            if(Equal(vec[i], values[0][idx], ri->type(idx), ri->length(idx)))
            {
                return false;
            }
        }
        notEqual.erase(ite++);
    }
    return true;
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
    if(recordInfoLength + (sizeof(int) * 4 + name.size()) > DataFileDescPage::maxRecordInfoLength())
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
