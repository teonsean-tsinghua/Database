#include"RecordSlot.h"

RecordSlot::RecordSlot(char* cache, RecordInfo* ri):
    cache(cache), ri(ri)
{
    actual_data_offset = ri->getFieldCount();
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
        if(readBool((*this)[idx]) != iter->second)
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
        if(readBool((*this)[idx]))
        {
            return false;
        }
        char* left = (*this)[offset + actual_data_offset], *right = (char*)iter->second;
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
        if(readBool((*this)[idx]))
        {
            return true;
        }
        std::vector<void*>& vecs = iter->second;
        for(int i = 0; i < vecs.size(); i++)
        {
            if(memcmp((*this)[offset + actual_data_offset], vecs[i], ri->length(idx)) == 0)
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
            lnull = readBool((*this)[lidx]);
            rnull = readBool((*this)[ridx]);
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
            char* left = (*this)[loffset + actual_data_offset], * right = (*this)[roffset + actual_data_offset];
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
    assert(cnt == data.size());
    for(int i = 0; i < cnt; i++)
    {
        int offset = ri->offset(i);
        char* ptr = (char*)(data[i]);
        if(ptr == NULL)
        {
            writeBool((*this)[i], true);
        }
        else
        {
            writeBool((*this)[i], false);
            copyData(ptr, (*this)[offset + actual_data_offset], ri->length(i));
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
            writeBool((*this)[idx], true);
        }
        else
        {
            writeBool((*this)[idx], false);
            copyData(ptr, (*this)[ri->offset(idx) + actual_data_offset], ri->length(idx));
        }
    }
}

void RecordSlot::print()
{
    int cnt = ri->getFieldCount();
    for(int i = 0; i < cnt; i++)
    {
        std::cout << ri->name(i) << ": ";
        switch(ri->type(i))
        {
        case Type::_ID:
            std::cout << read_id((*this)[actual_data_offset + ri->offset(i)]);
            break;
        case Type::INT:
            std::cout << readInt((*this)[actual_data_offset + ri->offset(i)]);
            break;
        case Type::FLOAT:
            std::cout << readFloat((*this)[actual_data_offset + ri->offset(i)]);
            break;
        case Type::VARCHAR:
            std::cout << (*this)[actual_data_offset + ri->offset(i)];
            break;
        case Type::DATE:
            std::cout << readString((*this)[actual_data_offset + ri->offset(i)], Type::typeSize(Type::DATE));
            break;
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}

char* RecordSlot::get(int i)
{
    return (*this)[actual_data_offset + ri->offset(i)];
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
        bool isNull = readBool((*this)[i]);
        if(ri->nullable(i) && isNull)
        {
            data[i] = NULL;
        }
        else
        {
            int len = ri->length(i);
            char* tmp = new char[len];
            copyData((*this)[offset + actual_data_offset], tmp, len);
            data[i] = (void*)tmp;
        }
    }
}

char* RecordSlot::operator[](const int offset) const
{
    return (cache + offset);
}
