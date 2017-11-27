#include"DBDataFileDescriptionSlot.h"

DBDataFileDescriptionSlot::DBDataFileDescriptionSlot(BufType cache, int mode, DBRecordInfo* ri):
    DBSlot(cache), ri(ri)
{
    firstDataPage = (*this)[FIRST_DATA_PAGE_OFFSET];
    firstUsagePage = (*this)[FIRST_USAGE_PAGE_OFFSET];
    pageNumber = (*this)[PAGE_NUMBER_OFFSET];
    primaryKeyIndex = (*this)[PRIMARY_KEY_INDEX_OFFSET];
    recordInfoLength = (*this)[RECORD_INFO_LENGTH_OFFSET];
    recordInfo = (*this)[RECORD_INFO_OFFSET];
    recordLength = 0;
    if(mode == MODE_PARSE)
    {
        char* cache = (char*)recordInfo;
        char* end = cache + getRecordInfoLength();
        int index = 0, offset = 0, type = 0, name_length = 0, nullable = 0;
        if(!ri->indexes.empty() ||
           !ri->names.empty() ||
           !ri->types.empty() ||
           !ri->nullables.empty() ||
           !ri->offsets.empty())
        {
            ri->indexes.clear();
            ri->names.clear();
            ri->types.clear();
            ri->nullables.clear();
            ri->offsets.clear();
        }
        while(cache < end)
        {
            readInt((BufType)(cache + RECORD_INFO_TYPE_OFFSET), &type);
            readInt((BufType)(cache + RECORD_INFO_NULLABLE_OFFSET), &nullable);
            readInt((BufType)(cache + RECORD_INFO_NAME_LENGTH_OFFSET), &name_length);
            cache += RECORD_INFO_NAME_OFFSET;
            std::string name;
            readString(cache, name, name_length);
            ri->indexes[name] = index;
            ri->names.push_back(name);
            ri->types.push_back(type);
            ri->offsets.push_back(offset);
            ri->nullables.push_back(nullable == 1 ? true : false);
            offset += (DBType::typeSize(type) + 1);
            cache += name_length;
            index++;
        }
        recordLength = ri->indexes.size() ? offset : 0;
        currentRecordInfoLength = getRecordInfoLength();
    }
    else if(mode == MODE_CREATE)
    {
        currentRecordInfoLength = 0;
        setRecordInfoLength(0);
        setPrimaryKeyIndex(0);
        recordLength = 0;
        ri->indexes.clear();
        ri->names.clear();
        ri->types.clear();
        ri->offsets.clear();
        ri->nullables.clear();
    }
}

int DBDataFileDescriptionSlot::size()
{
    return sizeof(int) * 5 + currentRecordInfoLength;
}

int DBDataFileDescriptionSlot::addField(std::string name, int type, bool nullable, char* boundary)
{
    if(ri->indexes.count(name))
    {
        return FIELD_ALREADY_EXIST;
    }
    if(name.empty())
    {
        return EMPTY_FIELD_NAME;
    }
    currentRecordInfoLength += (sizeof(int) * 3 + name.size());
    if((char*)recordInfo + currentRecordInfoLength >= boundary)
    {
        currentRecordInfoLength -= (sizeof(int) * 3 + name.size());
        return EXCEED_PAGE_LIMIT;
    }
    ri->names.push_back(name);
    ri->types.push_back(type);
    ri->offsets.push_back(recordLength);
    ri->nullables.push_back(nullable);
    recordLength += (DBType::typeSize(type) + 1);
    ri->indexes[name] = ri->names.size() - 1;
    return SUCCEED;
}

void DBDataFileDescriptionSlot::print()
{
    int cnt = ri->getFieldCount();
    DBPrint::print("This file has pages of number ").printLine(getPageNumber())
            .print("First data page is at ").printLine(getFirstDataPage())
            .print("First usage page is at ").printLine(getFirstUsagePage())
            .print("Record length is ").printLine(getRecordLength())
            .print("Record info length is ").printLine(getRecordInfoLength())
            .print("Number of fields: ").printLine(cnt)
            .print("Primary key is: ").printLine(getPrimaryKeyIndex());
    for(int i = 0; i < cnt; i++)
    {
        DBPrint::print("Field " + ri->names[i] + " is of type " + std::string(DBType::typeName(ri->types[i])));
        if(ri->nullables[i])
        {
            DBPrint::printLine(", and can be null.");
        }
        else
        {
            DBPrint::printLine(", and cannot be null.");
        }
    }
}

int DBDataFileDescriptionSlot::getRecordLength()
{
    return recordLength;
}

int DBDataFileDescriptionSlot::getFirstDataPage()
{
    int re;
    readInt(firstDataPage, &re);
    return re;
}

int DBDataFileDescriptionSlot::getFirstUsagePage()
{
    int re;
    readInt(firstUsagePage, &re);
    return re;
}

int DBDataFileDescriptionSlot::getPageNumber()
{
    int re;
    readInt(pageNumber, &re);
    return re;
}

int DBDataFileDescriptionSlot::getRecordInfoLength()
{
    int re;
    readInt(recordInfoLength, &re);
    return re;
}

int DBDataFileDescriptionSlot::getPrimaryKeyIndex()
{
    int re;
    readInt(primaryKeyIndex, &re);
    return re;
}

void DBDataFileDescriptionSlot::setFirstDataPage(int n)
{
    writeInt(firstDataPage, n);
}

void DBDataFileDescriptionSlot::setFirstUsagePage(int n)
{
    writeInt(firstUsagePage, n);
}

void DBDataFileDescriptionSlot::setPageNumber(int n)
{
    writeInt(pageNumber, n);
}

void DBDataFileDescriptionSlot::setRecordInfoLength(int n)
{
    writeInt(recordInfoLength, n);
}

void DBDataFileDescriptionSlot::setPrimaryKeyIndex(int n)
{
    writeInt(primaryKeyIndex, n);
}

void DBDataFileDescriptionSlot::write()
{
    setRecordInfoLength(currentRecordInfoLength);
    char* cache = (char*)recordInfo;
    if(ri->names.size() != ri->types.size())
    {
        throw ERROR;
    }
    for(int i = 0; i < ri->names.size(); i++)
    {
        int name_length = ri->names[i].size();
        writeInt((BufType)(cache + RECORD_INFO_TYPE_OFFSET), ri->types[i]);
        writeInt((BufType)(cache + RECORD_INFO_NULLABLE_OFFSET), ri->nullables[i] ? 1 : 0);
        writeInt((BufType)(cache + RECORD_INFO_NAME_LENGTH_OFFSET), name_length);
        cache += RECORD_INFO_NAME_OFFSET;
        writeString(cache, ri->names[i], name_length);
        cache += name_length;
    }
}
