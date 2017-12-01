#include"DBRecordSlot.h"

DBRecordSlot::DBRecordSlot(BufType cache, DBRecordInfo* ri):
    DBSlot(cache), ri(ri)
{

}

void DBRecordSlot::checkNull(std::map<int, bool>& nulls, std::list<std::vector<void*> >& datas)
{
    std::map<int, bool>::iterator iter;
    bool b;
    for(iter = nulls.begin(); iter != nulls.end(); iter++)
    {
        int idx = iter->first;
        readCharToBool((*this)[ri->offset(idx)], &b);
        if(b == iter->second)
        {
            datas.push_back(std::vector<void*>());
            read(datas.back());
        }
    }
}

void DBRecordSlot::checkValue(std::map<int, void*>& info, std::list<std::vector<void*> >& datas, int op)
{
    std::map<int, void*>::iterator iter;
    for(iter = info.begin(); iter != info.end(); iter++)
    {
        int idx = iter->first;
        int offset = ri->offset(idx);
        bool flag = false;
        readCharToBool((*this)[offset], &flag);
        if(flag)
        {
            return;
        }
        switch(op)
        {
        case 0:
            flag = Equal((*this)[offset + 1], iter->second, ri->type(idx), ri->length(idx));
            break;
        case 1:
            flag = !Equal((*this)[offset + 1], iter->second, ri->type(idx), ri->length(idx));
            break;
        case 2:
            flag = smallerOrEqual((*this)[offset + 1], iter->second, ri->type(idx), ri->length(idx));
            break;
        case 3:
            flag = largerOrEqual((*this)[offset + 1], iter->second, ri->type(idx), ri->length(idx));
            break;
        case 4:
            flag = smaller((*this)[offset + 1], iter->second, ri->type(idx), ri->length(idx));
            break;
        case 5:
            flag = larger((*this)[offset + 1], iter->second, ri->type(idx), ri->length(idx));
            break;
        }
        if(!flag)
        {
            return;
        }
    }
    datas.push_back(std::vector<void*>());
    read(datas.back());
}

void DBRecordSlot::checkFields(std::map<int, int>& info, std::list<std::vector<void*> >& datas, int op)
{
    std::map<int, int>::iterator iter;
    for(iter = info.begin(); iter != info.end(); iter++)
    {
        int lidx = iter->first;
        int loffset = ri->offset(lidx);
        int ridx = iter->second;
        int roffset = ri->offset(ridx);
        bool lnull, rnull;
        readCharToBool((*this)[loffset], &lnull);
        readCharToBool((*this)[roffset], &rnull);
        if(lnull && rnull)
        {
            if(op != 0)
            {
                return;
            }
        }
        if((lnull && !rnull) || (!lnull && rnull))
        {
            if(op != 1)
            {
                return;
            }
        }
        bool flag = false;
        switch(op)
        {
        case 0:
            flag = Equal((*this)[loffset + 1], (*this)[roffset + 1], ri->type(lidx), ri->length(lidx));
            break;
        case 1:
            flag = !Equal((*this)[loffset + 1], (*this)[roffset + 1], ri->type(lidx), ri->length(lidx));
            break;
        case 2:
            flag = smallerOrEqual((*this)[loffset + 1], (*this)[roffset + 1], ri->type(lidx), ri->length(lidx));
            break;
        case 3:
            flag = largerOrEqual((*this)[loffset + 1], (*this)[roffset + 1], ri->type(lidx), ri->length(lidx));
            break;
        case 4:
            flag = smaller((*this)[loffset + 1], (*this)[roffset + 1], ri->type(lidx), ri->length(lidx));
            break;
        case 5:
            flag = larger((*this)[loffset + 1], (*this)[roffset + 1], ri->type(lidx), ri->length(lidx));
            break;
        }
        if(!flag)
        {
            return;
        }
    }
    datas.push_back(std::vector<void*>());
    read(datas.back());
}

void DBRecordSlot::write(std::vector<void*>& data)
{
    int cnt = ri->getFieldCount();
    if(cnt != data.size())
    {
        throw Exception("Not enough fields provided for insertion.");
    }
    write_id((*this)[1]);
    for(int i = 1; i < cnt; i++)
    {
        int offset = ri->offset(i);
        char* ptr = (char*)(data[i]);
        if(ptr == NULL)
        {
            writeBoolToChar((*this)[offset], true);
        }
        else
        {
            writeBoolToChar((*this)[offset], false);
            switch(ri->type(i))
            {
            case DBType::INT:
                writeInt((*this)[offset + 1], *(int*)data[i]);
                break;
            default:
                writeData((*this)[offset + 1], ptr, ri->length(i));
            }
        }
    }
}

void DBRecordSlot::print()
{
    std::vector<void*> data;
    read(data);
    for(int idx = 0; idx < data.size(); idx++)
    {
        void* ptr = data[idx];
        DBPrint::print(ri->name(idx) + ": ");
        if(ptr == NULL)
        {
            DBPrint::printLine("NULL");
        }
        else
        {
            switch(ri->type(idx))
            {
            case DBType::_ID:
                DBPrint::print_ID((char*)ptr).printLine("");
                break;
            case DBType::INT:
                DBPrint::printLine(*(int*)ptr);
                break;
            case DBType::FLOAT:
                DBPrint::printLine(*(float*)ptr);
                break;
            case DBType::DATE:
                DBPrint::printLine(*(const char*)ptr);
                break;
            case DBType::VARCHAR:
                DBPrint::printLine("");
                break;
            }
        }
    }
    DBPrint::printLine("");
}

void DBRecordSlot::copy(DBRecordSlot* src, DBRecordSlot* dest, int length)
{
    copyData(src->cache, dest->cache, length);
}

void DBRecordSlot::update(std::map<int, void*>& data)
{
    std::map<int, void*>::iterator iter;
    for(iter = data.begin(); iter != data.end(); iter++)
    {
        int idx = iter->first;
        char* ptr = (char*)iter->second;
        if(ptr == NULL)
        {
            writeBoolToChar((*this)[ri->offset(idx)], true);
        }
        else
        {
            writeData((*this)[ri->offset(idx) + 1], ptr, ri->length(idx));
        }
    }
}

void DBRecordSlot::read(std::map<std::string, void*>& data)
{
    data.clear();
    int cnt = ri->getFieldCount();
    char* _id = new char[DBType::typeSize(DBType::_ID)];
    read_id((*this)[1], _id);
    data["_id"] = (void*)_id;
    for(int i = 1; i < cnt; i++)
    {
        int offset = ri->offset(i);
        bool isNull;
        readCharToBool((*this)[offset], &isNull);
        if(ri->nullable(i) && isNull)
        {
            data[ri->name(i)] = NULL;
        }
        else
        {
            int len = ri->length(i);
            char* tmp = new char[len];
            readData((*this)[offset + 1], tmp, len);
            data[ri->name(i)] = (void*)tmp;
        }
    }
}

bool DBRecordSlot::compare_id(char* _id)
{
    if(strncmp(_id, (char*)((*this)[1]), DBType::typeSize(DBType::_ID)) == 0)
    {
        return true;
    }
    return false;
}

void DBRecordSlot::get_id(char* _id)
{
    read_id((*this)[1], _id);
}

void DBRecordSlot::read(std::vector<void*>& data)
{
    data.clear();
    int cnt = ri->getFieldCount();
    data.assign(cnt, NULL);
    data[0] = (void*)((*this)[1]);
    for(int i = 1; i < cnt; i++)
    {
        int offset = ri->offset(i);
        bool isNull;
        readCharToBool((*this)[offset], &isNull);
        if(ri->nullable(i) && isNull)
        {
            data[i] = NULL;
        }
        else
        {
            int len = ri->length(i);
            char* tmp = new char[len];
            readData((*this)[offset + 1], tmp, len);
            data[i] = (void*)tmp;
        }
    }
}
