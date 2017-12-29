#include"RecordSlot.h"

RecordSlot::RecordSlot(char* cache, RecordInfo* ri):
    cache(cache), ri(ri)
{

}

bool RecordSlot::check(SearchInfo& si)
{
    if(!checkNull(si.nulls))
    {
        return false;
    }
    if(!checkValueNotEqual(si.notEqual))
    {
        return false;
    }
    for(int i = 0; i < 5; i++)
    {
        if(!checkValue(si.values[i], i))
        {
            return false;
        }
    }
    for(int i = 0; i < 6; i++)
    {
        if(!checkFields(si.fields[i], i))
        {
            return false;
        }
    }
    return true;
}

bool RecordSlot::checkNull(std::map<int, bool>& nulls)
{
    std::map<int, bool>::iterator iter;
    for(iter = nulls.begin(); iter != nulls.end(); iter++)
    {
        int idx = iter->first;
        if(readBool((*this)[ri->offset(idx)]) != iter->second)
        {
            return false;
        }
    }
    return true;
}

bool RecordSlot::checkValue(std::map<int, void*>& info, int op)
{
    std::map<int, void*>::iterator iter;
    for(iter = info.begin(); iter != info.end(); iter++)
    {
        int idx = iter->first;
        int offset = ri->offset(idx);
        if(readBool((*this)[offset]))
        {
            return false;
        }
        char* left = (*this)[offset + 1], *right = (char*)iter->second;
        bool flag = false;
        switch(op)
        {
        case 0:
            switch(ri->type(idx))
            {
            case Type::_ID:
                flag = (*(IDType*)left == *(IDType*)right);
                break;
            case Type::INT:
                flag = (*(IntType*)left == *(IntType*)right);
                break;
            case Type::FLOAT:
                flag = (*(FloatType*)left == *(FloatType*)right);
                break;
            case Type::DATE:
                flag = (*(DateType*)left == *(DateType*)right);
                break;
            case Type::VARCHAR:
                flag = (*(VarcharType*)left == *(VarcharType*)right);
                break;
            }
            break;
        case 1:
            switch(ri->type(idx))
            {
            case Type::_ID:
                flag = (*(IDType*)left <= *(IDType*)right);
                break;
            case Type::INT:
                flag = (*(IntType*)left <= *(IntType*)right);
                break;
            case Type::FLOAT:
                flag = (*(FloatType*)left <= *(FloatType*)right);
                break;
            case Type::DATE:
                flag = (*(DateType*)left <= *(DateType*)right);
                break;
            case Type::VARCHAR:
                flag = (*(VarcharType*)left <= *(VarcharType*)right);
                break;
            }
            break;
        case 2:
            switch(ri->type(idx))
            {
            case Type::_ID:
                flag = (*(IDType*)left >= *(IDType*)right);
                break;
            case Type::INT:
                flag = (*(IntType*)left >= *(IntType*)right);
                break;
            case Type::FLOAT:
                flag = (*(FloatType*)left >= *(FloatType*)right);
                break;
            case Type::DATE:
                flag = (*(DateType*)left >= *(DateType*)right);
                break;
            case Type::VARCHAR:
                flag = (*(VarcharType*)left >= *(VarcharType*)right);
                break;
            }
            break;
        case 3:
            switch(ri->type(idx))
            {
            case Type::_ID:
                flag = (*(IDType*)left < *(IDType*)right);
                break;
            case Type::INT:
                flag = (*(IntType*)left < *(IntType*)right);
                break;
            case Type::FLOAT:
                flag = (*(FloatType*)left < *(FloatType*)right);
                break;
            case Type::DATE:
                flag = (*(DateType*)left < *(DateType*)right);
                break;
            case Type::VARCHAR:
                flag = (*(VarcharType*)left < *(VarcharType*)right);
                break;
            }
            break;
        case 4:
            switch(ri->type(idx))
            {
            case Type::_ID:
                flag = (*(IDType*)left > *(IDType*)right);
                break;
            case Type::INT:
                flag = (*(IntType*)left > *(IntType*)right);
                break;
            case Type::FLOAT:
                flag = (*(FloatType*)left > *(FloatType*)right);
                break;
            case Type::DATE:
                flag = (*(DateType*)left > *(DateType*)right);
                break;
            case Type::VARCHAR:
                flag = (*(VarcharType*)left > *(VarcharType*)right);
                break;
            }
            break;
        }
        if(!flag)
        {
            return false;
        }
    }
    return true;
}

bool RecordSlot::checkValueNotEqual(std::map<int, std::vector<void*> >& info)
{
    std::map<int, vector<void*> >::iterator iter;
    for(iter = info.begin(); iter != info.end(); iter++)
    {
        int idx = iter->first;
        int offset = ri->offset(idx);
        if(readBool((*this)[offset]))
        {
            return true;
        }
        std::vector<void*>& vecs = iter->second;
        for(int i = 0; i < vecs.size(); i++)
        {
            if(memcmp((*this)[offset + 1], vecs[i], ri->length(idx)) == 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool RecordSlot::checkFields(std::map<int, std::vector<int> >& info, int op)
{
    std::map<int, std::vector<int> >::iterator iter;
    for(iter = info.begin(); iter != info.end(); iter++)
    {
        std::vector<int>& vecs = iter->second;
        for(int i = 0; i < vecs.size(); i++)
        {
            int lidx = iter->first;
            int loffset = ri->offset(lidx);
            int ridx = vecs[i];
            int roffset = ri->offset(ridx);
            bool lnull, rnull;
            lnull = readBool((*this)[loffset]);
            rnull = readBool((*this)[roffset]);
            if(lnull && rnull)
            {
                if(op != 0)
                {
                    return false;
                }
            }
            if((lnull && !rnull) || (!lnull && rnull))
            {
                if(op != 1)
                {
                    return false;
                }
            }
            bool flag = false;
            char* left = (*this)[loffset + 1], * right = (*this)[roffset + 1];
            switch(op)
            {
            case 0:
                switch(ri->type(lidx))
                {
                case Type::_ID:
                    flag = (*(IDType*)left == *(IDType*)right);
                    break;
                case Type::INT:
                    flag = (*(IntType*)left == *(IntType*)right);
                    break;
                case Type::FLOAT:
                    flag = (*(FloatType*)left == *(FloatType*)right);
                    break;
                case Type::DATE:
                    flag = (*(DateType*)left == *(DateType*)right);
                    break;
                case Type::VARCHAR:
                    flag = (*(VarcharType*)left == *(VarcharType*)right);
                    break;
                }
                break;
            case 1:
                switch(ri->type(lidx))
                {
                case Type::_ID:
                    flag = (*(IDType*)left <= *(IDType*)right);
                    break;
                case Type::INT:
                    flag = (*(IntType*)left <= *(IntType*)right);
                    break;
                case Type::FLOAT:
                    flag = (*(FloatType*)left <= *(FloatType*)right);
                    break;
                case Type::DATE:
                    flag = (*(DateType*)left <= *(DateType*)right);
                    break;
                case Type::VARCHAR:
                    flag = (*(VarcharType*)left <= *(VarcharType*)right);
                    break;
                }
                break;
            case 2:
                switch(ri->type(lidx))
                {
                case Type::_ID:
                    flag = (*(IDType*)left >= *(IDType*)right);
                    break;
                case Type::INT:
                    flag = (*(IntType*)left >= *(IntType*)right);
                    break;
                case Type::FLOAT:
                    flag = (*(FloatType*)left >= *(FloatType*)right);
                    break;
                case Type::DATE:
                    flag = (*(DateType*)left >= *(DateType*)right);
                    break;
                case Type::VARCHAR:
                    flag = (*(VarcharType*)left >= *(VarcharType*)right);
                    break;
                }
                break;
            case 3:
                switch(ri->type(lidx))
                {
                case Type::_ID:
                    flag = (*(IDType*)left < *(IDType*)right);
                    break;
                case Type::INT:
                    flag = (*(IntType*)left < *(IntType*)right);
                    break;
                case Type::FLOAT:
                    flag = (*(FloatType*)left < *(FloatType*)right);
                    break;
                case Type::DATE:
                    flag = (*(DateType*)left < *(DateType*)right);
                    break;
                case Type::VARCHAR:
                    flag = (*(VarcharType*)left < *(VarcharType*)right);
                    break;
                }
                break;
            case 4:
                switch(ri->type(lidx))
                {
                case Type::_ID:
                    flag = (*(IDType*)left > *(IDType*)right);
                    break;
                case Type::INT:
                    flag = (*(IntType*)left > *(IntType*)right);
                    break;
                case Type::FLOAT:
                    flag = (*(FloatType*)left > *(FloatType*)right);
                    break;
                case Type::DATE:
                    flag = (*(DateType*)left > *(DateType*)right);
                    break;
                case Type::VARCHAR:
                    flag = (*(VarcharType*)left > *(VarcharType*)right);
                    break;
                }
                break;
            case 5:
                switch(ri->type(lidx))
                {
                case Type::_ID:
                    flag = (*(IDType*)left != *(IDType*)right);
                    break;
                case Type::INT:
                    flag = (*(IntType*)left != *(IntType*)right);
                    break;
                case Type::FLOAT:
                    flag = (*(FloatType*)left != *(FloatType*)right);
                    break;
                case Type::DATE:
                    flag = (*(DateType*)left != *(DateType*)right);
                    break;
                case Type::VARCHAR:
                    flag = (*(VarcharType*)left != *(VarcharType*)right);
                    break;
                }
                break;
            }
            if(!flag)
            {
                return false;
            }
        }
    }
    return true;
}

void RecordSlot::write(std::vector<void*>& data)
{
    int cnt = ri->getFieldCount();
    assert(cnt != data.size());
    writeBool((*this)[0], false);
    write_id((*this)[1]);
    for(int i = 1; i < cnt; i++)
    {
        int offset = ri->offset(i);
        char* ptr = (char*)(data[i]);
        if(ptr == NULL)
        {
            writeBool((*this)[offset], true);
        }
        else
        {
            writeBool((*this)[offset], false);
            copyData(ptr, (*this)[offset + 1], ri->length(i));
        }
    }
}

void RecordSlot::copy(RecordSlot* src, RecordSlot* dest, int length)
{
    copyData(src->cache, dest->cache, length);
}

void RecordSlot::update(UpdateInfo& ui)
{
    std::map<int, void*>::iterator iter;
    for(iter = ui.data.begin(); iter != ui.data.end(); iter++)
    {
        int idx = iter->first;
        char* ptr = (char*)iter->second;
        if(ptr == NULL)
        {
            writeBool((*this)[ri->offset(idx)], true);
        }
        else
        {
            writeBool((*this)[ri->offset(idx)], false);
            copyData(ptr, (*this)[ri->offset(idx) + 1], ri->length(idx));
        }
    }
}

void RecordSlot::print()
{

}

void RecordSlot::read(std::vector<void*>& data)
{
    data.clear();
    int cnt = ri->getFieldCount();
    data.assign(cnt, NULL);
    data[0] = (void*)((*this)[1]);
    for(int i = 1; i < cnt; i++)
    {
        int offset = ri->offset(i);
        bool isNull = readBool((*this)[offset]);
        if(ri->nullable(i) && isNull)
        {
            data[i] = NULL;
        }
        else
        {
            int len = ri->length(i);
            char* tmp = new char[len];
            copyData((*this)[offset + 1], tmp, len);
            data[i] = (void*)tmp;
        }
    }
}

char* RecordSlot::operator[](const int offset) const
{
    return (cache + offset);
}
