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

bool RecordSlot::checkValue(std::map<int, const void*>& info, int op)
{
    std::map<int, const void*>::iterator iter;
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

bool RecordSlot::checkValueNotEqual(std::map<int, std::vector<const void*> >& info)
{
    std::map<int, vector<const void*> >::iterator iter;
    for(iter = info.begin(); iter != info.end(); iter++)
    {
        int idx = iter->first;
        int offset = ri->offset(idx);
        if(readBool((*this)[idx]))
        {
            return true;
        }
        std::vector<const void*>& vecs = iter->second;
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

void RecordSlot::assignValue(int idx, const void* value)
{
    if(value == NULL)
    {
        writeBool((*this)[idx], true);
        return;
    }
    writeBool((*this)[idx], false);
    copyData((const char*)value, (*this)[actual_data_offset + ri->offset(idx)], ri->length(idx));
}

void RecordSlot::update(UpdateInfo& ui)
{
    for(std::map<int, SetRValue>::iterator iter = ui.action.begin(); iter != ui.action.end(); iter++)
    {
        SetRValue& action = iter->second;
        int idx = iter->first;
        switch(action.type)
        {
        case 1:
        {
            const void* value;
            switch(action.value.type)
            {
            case 0:
                value = NULL;
                break;
            case 1:
                value = &action.value.v_int;
                break;
            case 2:
                value = action.value.v_str.c_str();
                break;
            case 3:
                value = &action.value.v_float;
                break;
            }
            assignValue(idx, value);
            break;
        }
        case 2:
        {
            int idx2 = ri->index(action.col);
            if(readBool((*this)[idx2]))
            {
                assignValue(idx, NULL);
            }
            else
            {
                assignValue(idx, (*this)[actual_data_offset + ri->offset(idx2)]);
            }
            break;
        }
        case 3:
        {
            int lidx = ri->index(action.col), ridx = ri->index(action.col2);
            int type = ri->type(idx) * 100 + ri->type(lidx) * 10 + ri->type(ridx);
            writeBool((*this)[idx], false);
            switch(type)
            {
            case 111:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 112:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 121:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 122:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 211:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 212:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 221:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 222:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            }
            break;
        }
        case 4:
        {
            int lidx = ri->index(action.col), ridx = ri->index(action.col2);
            int type = ri->type(idx) * 100 + ri->type(lidx) * 10 + ri->type(ridx);
            writeBool((*this)[idx], false);
            switch(type)
            {
            case 111:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] -
                          *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 112:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] -
                          *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 121:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] -
                          *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 122:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] -
                          *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 211:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] -
                            *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 212:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] -
                            *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 221:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] -
                            *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 222:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] -
                            *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            }
            break;
        }
        case 5:
        {
            int lidx = ri->index(action.col), ridx = ri->index(action.col2);
            int type = ri->type(idx) * 100 + ri->type(lidx) * 10 + ri->type(ridx);
            writeBool((*this)[idx], false);
            switch(type)
            {
            case 111:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] *
                          *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 112:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] *
                          *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 121:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] *
                          *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 122:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] *
                          *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 211:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] *
                            *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 212:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] *
                            *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 221:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] *
                            *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 222:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] *
                            *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            }
            break;
        }
        case 6:
        {
            int lidx = ri->index(action.col), ridx = ri->index(action.col2);
            int type = ri->type(idx) * 100 + ri->type(lidx) * 10 + ri->type(ridx);
            writeBool((*this)[idx], false);
            switch(type)
            {
            case 111:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] /
                          *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 112:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] /
                          *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 121:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] /
                          *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 122:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] /
                          *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 211:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] /
                            *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 212:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] /
                            *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 221:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] /
                            *(int*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            case 222:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] /
                            *(float*)(*this)[actual_data_offset + ri->offset(ridx)]);
                break;
            }
            }
            break;
        }
        case 7:
        {
            int lidx = ri->index(action.col);
            int type = ri->type(idx) * 100 + ri->type(lidx) * 10 + action.value.type;
            writeBool((*this)[idx], false);
            switch(type)
            {
            case 111:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          action.value.v_int);
                break;
            }
            case 113:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          action.value.v_float);
                break;
            }
            case 121:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          action.value.v_int);
                break;
            }
            case 123:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          action.value.v_float);
                break;
            }
            case 211:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            action.value.v_int);
                break;
            }
            case 213:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            action.value.v_float);
                break;
            }
            case 221:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            action.value.v_int);
                break;
            }
            case 223:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            action.value.v_float);
                break;
            }
            }
            break;
        }
        case 8:
        {
            int lidx = ri->index(action.col);
            int type = ri->type(idx) * 100 + ri->type(lidx) * 10 + action.value.type;
            writeBool((*this)[idx], false);
            switch(type)
            {
            case 111:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] -
                          action.value.v_int);
                break;
            }
            case 113:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] -
                          action.value.v_float);
                break;
            }
            case 121:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] -
                          action.value.v_int);
                break;
            }
            case 123:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] -
                          action.value.v_float);
                break;
            }
            case 211:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] -
                            action.value.v_int);
                break;
            }
            case 213:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] -
                            action.value.v_float);
                break;
            }
            case 221:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] -
                            action.value.v_int);
                break;
            }
            case 223:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] -
                            action.value.v_float);
                break;
            }
            }
            break;
        }
        case 9:
        {
            int lidx = ri->index(action.col);
            int type = ri->type(idx) * 100 + ri->type(lidx) * 10 + action.value.type;
            writeBool((*this)[idx], false);
            switch(type)
            {
            case 111:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] *
                          action.value.v_int);
                break;
            }
            case 113:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] *
                          action.value.v_float);
                break;
            }
            case 121:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] *
                          action.value.v_int);
                break;
            }
            case 123:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] *
                          action.value.v_float);
                break;
            }
            case 211:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] *
                            action.value.v_int);
                break;
            }
            case 213:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] *
                            action.value.v_float);
                break;
            }
            case 221:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] *
                            action.value.v_int);
                break;
            }
            case 223:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] *
                            action.value.v_float);
                break;
            }
            }
            break;
        }
        case 10:
        {
            int lidx = ri->index(action.col);
            int type = ri->type(idx) * 100 + ri->type(lidx) * 10 + action.value.type;
            writeBool((*this)[idx], false);
            switch(type)
            {
            case 111:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] /
                          action.value.v_int);
                break;
            }
            case 113:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(int*)(*this)[actual_data_offset + ri->offset(lidx)] /
                          action.value.v_float);
                break;
            }
            case 121:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] /
                          action.value.v_int);
                break;
            }
            case 123:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          *(float*)(*this)[actual_data_offset + ri->offset(lidx)] /
                          action.value.v_float);
                break;
            }
            case 211:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] /
                            action.value.v_int);
                break;
            }
            case 213:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(int*)(*this)[actual_data_offset + ri->offset(lidx)] /
                            action.value.v_float);
                break;
            }
            case 221:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] /
                            action.value.v_int);
                break;
            }
            case 223:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            *(float*)(*this)[actual_data_offset + ri->offset(lidx)] /
                            action.value.v_float);
                break;
            }
            }
            break;
        }
        case 11:
        {
            int lidx = ri->index(action.col);
            int type = ri->type(idx) * 100 + ri->type(lidx) * 10 + action.value.type;
            writeBool((*this)[idx], false);
            switch(type)
            {
            case 111:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          action.value.v_int / *(int*)(*this)[actual_data_offset + ri->offset(lidx)]);
                break;
            }
            case 113:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          action.value.v_float / *(int*)(*this)[actual_data_offset + ri->offset(lidx)]);
                break;
            }
            case 121:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          action.value.v_int / *(float*)(*this)[actual_data_offset + ri->offset(lidx)]);
                break;
            }
            case 123:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          action.value.v_float / *(float*)(*this)[actual_data_offset + ri->offset(lidx)]);
                break;
            }
            case 211:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            action.value.v_int / *(int*)(*this)[actual_data_offset + ri->offset(lidx)]);
                break;
            }
            case 213:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            action.value.v_float / *(int*)(*this)[actual_data_offset + ri->offset(lidx)]);
                break;
            }
            case 221:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            action.value.v_int / *(float*)(*this)[actual_data_offset + ri->offset(lidx)]);
                break;
            }
            case 223:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            action.value.v_float / *(float*)(*this)[actual_data_offset + ri->offset(lidx)]);
                break;
            }
            }
            break;
        }
        case 12:
        {
            int lidx = ri->index(action.col);
            int type = ri->type(idx) * 100 + ri->type(lidx) * 10 + action.value.type;
            writeBool((*this)[idx], false);
            switch(type)
            {
            case 111:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          -*(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          action.value.v_int);
                break;
            }
            case 113:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          -*(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          action.value.v_float);
                break;
            }
            case 121:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          -*(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          action.value.v_int);
                break;
            }
            case 123:
            {
                writeInt((*this)[actual_data_offset + ri->offset(idx)],
                          -*(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                          action.value.v_float);
                break;
            }
            case 211:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            -*(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            action.value.v_int);
                break;
            }
            case 213:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            -*(int*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            action.value.v_float);
                break;
            }
            case 221:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            -*(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            action.value.v_int);
                break;
            }
            case 223:
            {
                writeFloat((*this)[actual_data_offset + ri->offset(idx)],
                            -*(float*)(*this)[actual_data_offset + ri->offset(lidx)] +
                            action.value.v_float);
                break;
            }
            }
            break;
        }
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
    data[0] = (void*)((*this)[actual_data_offset]);
    for(int i = 1; i < cnt; i++)
    {
        if(ri->nullable(i) && readBool((*this)[i]))
        {
            data[i] = NULL;
        }
        else
        {
            // int len = ri->length(i);
            // char* tmp = new char[len];
            // copyData((*this)[offset + actual_data_offset], tmp, len);
            // data[i] = (void*)tmp;
            data[i] = (void*)((*this)[actual_data_offset + ri->offset(i)]);
        }
    }
}

char* RecordSlot::operator[](const int offset) const
{
    return (cache + offset);
}
