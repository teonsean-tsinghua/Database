#include"DBDataFileDescriptionSlot.h"

DBDataFileDescriptionSlot::DBDataFileDescriptionSlot(BufType cache, int mode):
    DBSlot(cache)
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
        if(!indexes.empty() ||
           !names.empty() ||
           !types.empty() ||
           !nullables.empty() ||
           !offsets.empty())
        {
            indexes.clear();
            names.clear();
            types.clear();
            nullables.clear();
            offsets.clear();
        }
        while(cache < end)
        {
            readInt((BufType)(cache + RECORD_INFO_TYPE_OFFSET), &type);
            readInt((BufType)(cache + RECORD_INFO_NULLABLE_OFFSET), &nullable);
            readInt((BufType)(cache + RECORD_INFO_NAME_LENGTH_OFFSET), &name_length);
            cache += RECORD_INFO_NAME_OFFSET;
            std::string name;
            readString(cache, name, name_length);
            indexes[name] = index;
            names.push_back(name);
            types.push_back(type);
            offsets.push_back(offset);
            nullables.push_back(nullable);
            offset += (DBType::typeSize(type) + 1);
            cache += name_length;
            index++;
        }
        recordLength = indexes.size() ? offset : 0;
        currentRecordInfoLength = getRecordInfoLength();
    }
    else if(mode == MODE_CREATE)
    {
        currentRecordInfoLength = 0;
        setRecordInfoLength(0);
        setPrimaryKeyIndex(0);
        recordLength = 0;
        indexes.clear();
        names.clear();
        types.clear();
        offsets.clear();
        nullables.clear();
    }
}

int DBDataFileDescriptionSlot::size()
{
    return sizeof(int) * 5 + currentRecordInfoLength;
}

int DBDataFileDescriptionSlot::addField(std::string name, int type, int nullable, char* boundary)
{
    if(indexes.count(name))
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
    names.push_back(name);
    types.push_back(type);
    offsets.push_back(recordLength);
    nullables.push_back(nullable);
    recordLength += (DBType::typeSize(type) + 1);
    indexes[name] = names.size() - 1;
    return SUCCEED;
}

int DBDataFileDescriptionSlot::getFieldCount()
{
    if(indexes.size() != names.size() ||
       names.size() != types.size() ||
       types.size() != offsets.size() ||
       offsets.size() != nullables.size())
    {
        return -1;
    }
    return indexes.size();
}

void DBDataFileDescriptionSlot::print()
{
    DBPrint("This file has pages of number ");
    DBPrintLine(getPageNumber());
    DBPrint("First data page is at ");
    DBPrintLine(getFirstDataPage());
    DBPrint("First usage page is at ");
    DBPrintLine(getFirstUsagePage());
    DBPrint("Record length is ");
    DBPrintLine(getRecordLength());
    DBPrint("Record info length is ");
    DBPrintLine(getRecordInfoLength());
    int cnt = getFieldCount();
    DBPrint("Number of fields: ");
    DBPrintLine(cnt);
    DBPrint("Primary key is: ");
    DBPrintLine(names[getPrimaryKeyIndex()]);
    for(int i = 0; i < cnt; i++)
    {
        DBPrint("Field " + names[i] + " is of type " + std::string(DBType::typeName(types[i])));
        if(nullables[i] != 0)
        {
            DBPrintLine(", and can be null.");
        }
        else
        {
            DBPrintLine(", and cannot be null.");
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

int DBDataFileDescriptionSlot::getOffsetOfField(int index)
{
    if(index >= 0 && index < offsets.size())
    {
        return offsets[index];
    }
    return -1;
}

int DBDataFileDescriptionSlot::getTypeOfField(int index)
{
    if(index >= 0 && index < types.size())
    {
        return types[index];
    }
    return -1;
}

std::string DBDataFileDescriptionSlot::getNameOfField(int index)
{
    if(index >= 0 && index < names.size())
    {
        return names[index];
    }
    return "";
}

int DBDataFileDescriptionSlot::getIndexOfField(std::string name)
{
    if(indexes.count(name))
    {
        return indexes[name];
    }
    return -1;
}

int DBDataFileDescriptionSlot::getNullableOfField(int index)
{
    if(index >= 0 && index < nullables.size())
    {
        return nullables[index];
    }
    return -1;
}

void DBDataFileDescriptionSlot::write()
{
    setRecordInfoLength(currentRecordInfoLength);
    char* cache = (char*)recordInfo;
    if(names.size() != types.size())
    {
        throw ERROR;
    }
    for(int i = 0; i < names.size(); i++)
    {
        int name_length = names[i].size();
        writeInt((BufType)(cache + RECORD_INFO_TYPE_OFFSET), types[i]);
        writeInt((BufType)(cache + RECORD_INFO_NULLABLE_OFFSET), nullables[i]);
        writeInt((BufType)(cache + RECORD_INFO_NAME_LENGTH_OFFSET), name_length);
        cache += RECORD_INFO_NAME_OFFSET;
        writeString(cache, names[i], name_length);
        cache += name_length;
    }
}
