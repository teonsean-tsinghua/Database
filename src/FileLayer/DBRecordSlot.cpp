#include"DBRecordSlot.h"

DBRecordSlot::DBRecordSlot(BufType cache):
    DBSlot(cache)
{
    ri = DBRecordInfo::getInstance();
}

int DBRecordSlot::write(std::vector<void*>& data)
{
    int cnt = ri->getFieldCount();
    if(cnt != data.size())
    {
        return ERROR;
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
    return SUCCEED;
}

void DBRecordSlot::print()
{
    std::vector<void*> data;
    if(read(data) != SUCCEED)
    {
        return;
    }
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

int DBRecordSlot::update(std::map<int, void*>& data)
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
    return SUCCEED;
}

int DBRecordSlot::equal(std::map<int, void*>& data)
{
    std::map<int, void*>::iterator iter;
    for(iter = data.begin(); iter != data.end(); iter++)
    {
        int idx = iter->first;
        char* ptr = (char*)iter->second;
        if(ptr == NULL)
        {
            bool isNull;
            readCharToBool((*this)[ri->offset(idx)], &isNull);
            if(!isNull)
            {
                return NON_EQUAL_RECORD;
            }
        }
        else if(strncmp(ptr, (char*)(*this)[ri->offset(idx) + 1], ri->length(idx)) != 0)
        {
            return NON_EQUAL_RECORD;
        }
    }
    return EQUAL_RECORD;
}

int DBRecordSlot::read(std::map<std::string, void*>& data)
{
    data.clear();
    int cnt = ri->getFieldCount();
    if(cnt < 0)
    {
        return ERROR;
    }
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
    return SUCCEED;
}

int DBRecordSlot::compare_id(char* _id)
{
    if(strncmp(_id, (char*)((*this)[1]), DBType::typeSize(DBType::_ID)) == 0)
    {
        return EQUAL_RECORD;
    }
    return NON_EQUAL_RECORD;
}

int DBRecordSlot::get_id(char* _id)
{
    read_id((*this)[1], _id);
    return SUCCEED;
}

int DBRecordSlot::read(std::vector<void*>& data)
{
    data.clear();
    int cnt = ri->getFieldCount();
    if(cnt < 0)
    {
        return ERROR;
    }
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
    return SUCCEED;
}
