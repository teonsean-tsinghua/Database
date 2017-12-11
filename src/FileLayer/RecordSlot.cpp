#include"RecordSlot.h"

RecordSlot::RecordSlot(BufType cache, RecordInfo* ri):
    Slot(cache), ri(ri)
{

}

bool RecordSlot::checkNull(std::map<int, bool>& nulls)
{
    std::map<int, bool>::iterator iter;
    bool b;
    for(iter = nulls.begin(); iter != nulls.end(); iter++)
    {
        int idx = iter->first;
        readCharToBool((*this)[ri->offset(idx)], &b);
        if(b != iter->second)
        {
            return false;
        }
    }
    return true;
}

bool RecordSlot::checkValue(std::map<int, std::vector<void*> >& info, int op)
{
    std::map<int, vector<void*> >::iterator iter;
    for(iter = info.begin(); iter != info.end(); iter++)
    {
        int idx = iter->first;
        int offset = ri->offset(idx);
        bool flag = false;
        readCharToBool((*this)[offset], &flag);
        if(flag)
        {
            return false;
        }
        std::vector<void*>& vecs = iter->second;
        for(int i = 0; i < vecs.size(); i++)
        {
            switch(op)
            {
            case 0:
                flag = Equal((*this)[offset + 1], vecs[i], ri->type(idx), ri->length(idx));
                break;
            case 1:
                flag = !Equal((*this)[offset + 1], vecs[i], ri->type(idx), ri->length(idx));
                break;
            case 2:
                flag = smallerOrEqual((*this)[offset + 1], vecs[i], ri->type(idx), ri->length(idx));
                break;
            case 3:
                flag = largerOrEqual((*this)[offset + 1], vecs[i], ri->type(idx), ri->length(idx));
                break;
            case 4:
                flag = smaller((*this)[offset + 1], vecs[i], ri->type(idx), ri->length(idx));
                break;
            case 5:
                flag = larger((*this)[offset + 1], vecs[i], ri->type(idx), ri->length(idx));
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
            readCharToBool((*this)[loffset], &lnull);
            readCharToBool((*this)[roffset], &rnull);
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
                return false;
            }
        }
    }
    return true;
}

void RecordSlot::write(std::vector<void*>& data)
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
            writeData((*this)[offset + 1], ptr, ri->length(i));
        }
    }
}

void RecordSlot::print()
{
    std::vector<void*> data;
    read(data);
    for(int idx = 0; idx < data.size(); idx++)
    {
        void* ptr = data[idx];
        Print::print(ri->name(idx) + ": ");
        if(ptr == NULL)
        {
            Print::printLine("NULL");
        }
        else
        {
            switch(ri->type(idx))
            {
            case Type::_ID:
                Print::print_ID((char*)ptr).printLine("");
                break;
            case Type::INT:
                Print::printLine(*(int*)ptr);
                break;
            case Type::FLOAT:
                Print::printLine(*(float*)ptr);
                break;
            case Type::DATE:
                Print::printLine(*(const char*)ptr);
                break;
            case Type::VARCHAR:
                Print::printLine("");
                break;
            }
        }
    }
    Print::printLine("");
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
            writeBoolToChar((*this)[ri->offset(idx)], true);
        }
        else
        {
            writeBoolToChar((*this)[ri->offset(idx)], false);
            writeData((*this)[ri->offset(idx) + 1], ptr, ri->length(idx));
        }
    }
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
