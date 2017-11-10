#include"DBRecordSlot.h"

DBRecordSlot::DBRecordSlot(BufType cache, DBRecordInfo* ri):
    DBSlot(cache), ri(ri)
{

}

int DBRecordSlot::write(std::vector<void*>& data)
{
    int cnt = ri->getFieldCount();
    if(cnt != data.size())
    {
        return ERROR;
    }
    for(int i = 1; i < cnt; i++)
    {
        int offset = ri->offsets[i];
        char* ptr = (char*)(data[i]);
        if(ptr == NULL)
        {
            writeBoolToChar((*this)[offset], true);
        }
        else
        {
            writeBoolToChar((*this)[offset], false);
            writeData((*this)[offset + 1], ptr, DBType::typeSize(ri->types[i]));
        }
    }
    return SUCCEED;
}

void DBRecordSlot::print()
{
    std::map<int, void*> data;
    if(read(data) != SUCCEED)
    {
        return;
    }
    for(std::map<int, void*>::iterator iter = data.begin(); iter != data.end(); iter++)
    {
        int idx = iter->first;
        void* ptr = iter->second;
        DBPrint(ri->names[idx] + ": ");
        if(ptr == NULL)
        {
            DBPrintLine("NULL");
        }
        else
        {
            switch(ri->types[idx])
            {
            case DBType::_ID:
                DBPrintLine(*(unsigned long long*)ptr);
                break;
            case DBType::INT:
                DBPrintLine(*(int*)ptr);
                break;
            }
        }
    }
}

int DBRecordSlot::read(std::map<int, void*>& data)
{
    data.clear();
    int cnt = ri->getFieldCount();
    if(cnt < 0)
    {
        return ERROR;
    }
    for(int i = 0; i < cnt; i++)
    {
        int offset = ri->offsets[i];
        bool isNull;
        readCharToBool((*this)[offset], &isNull);
        if(ri->nullables[i] && isNull)
        {
            data[i] = NULL;
        }
        else
        {
            int len = DBType::typeSize(ri->types[i]);
            char* tmp = new char[len];
            readData((*this)[offset + 1], tmp, len);
            data[i] = (void*)tmp;
        }
    }
    return SUCCEED;
}
