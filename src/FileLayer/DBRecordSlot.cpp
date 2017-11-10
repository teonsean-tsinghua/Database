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

int DBRecordSlot::read(std::map<std::string, void*>& data)
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
            data[ri->names[i]] = NULL;
        }
        else
        {
            int len = DBType::typeSize(ri->types[i]);
            char* tmp = new char[len];
            readData((*this)[offset + 1], tmp, len);
            data[ri->names[i]] = (void*)tmp;
        }
    }
    return SUCCEED;
}
