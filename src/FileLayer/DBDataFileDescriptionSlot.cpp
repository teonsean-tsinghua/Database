#include"DBDataFileDescriptionSlot.h"

DBDataFileDescriptionSlot::DBDataFileDescriptionSlot(BufType cache, int parse):
    DBSlot(cache, 0, parse)
{
    firstDataPage = (*this)[FIRST_DATA_PAGE_OFFSET];
    firstUsageSlot = (*this)[FIRST_USAGE_SLOT_OFFSET];
    lastUsageSlot = (*this)[LAST_USAGE_SLOT_OFFSET];
    recordInfoLength = (*this)[RECORD_INFO_LENGTH_OFFSET];
    recordInfo = (*this)[RECORD_INFO_OFFSET];
    if(parse)
    {
        char* cache = (char*)recordInfo;
        char* end = cache + getRecordInfoLength();
        int index = 0, offset = 0, type = 0, name_length = 0;
        if(!indexes.empty() ||
           !names.empty() ||
           !types.empty() ||
           !offsets.empty())
        {
            throw ERROR;
        }
        while(cache < end)
        {
            sscanf(cache + RECORD_INFO_TYPE_OFFSET, "%d", &type);
            sscanf(cache + RECORD_INFO_NAME_LENGTH_OFFSET, "%d", &name_length);
            cache += RECORD_INFO_NAME_OFFSET;
            std::string name;
            name = name.assign(cache, name_length);
            indexes[name] = index;
            names.push_back(name);
            types.push_back(type);
            offsets.push_back(offset);
            offset += DBType::typeSize(type);
            cache += name_length;
            index++;
        }
        recordLength = indexes.size() ? offsets[offsets.size() - 1] + DBType::typeSize(types[types.size() - 1]) : 0;
        currentRecordInfoLength = getRecordInfoLength();
    }
    else
    {
        currentRecordInfoLength = 0;
        setRecordInfoLength(0);
        recordLength = 0;
        indexes.clear();
        names.clear();
        types.clear();
        offsets.clear();
    }
}

int DBDataFileDescriptionSlot::size()
{
    return sizeof(int) * 4 + currentRecordInfoLength;
}

int DBDataFileDescriptionSlot::addField(std::string name, int type)
{
    if(indexes.count(name))
    {
        return FIELD_ALREADY_EXIST;
    }
    names.push_back(name);
    types.push_back(type);
    offsets.push_back(recordLength);
    recordLength += DBType::typeSize(type);
    indexes[name] = names.size();
    currentRecordInfoLength += (sizeof(int) * 2 + name.size());
    setRecordInfoLength(currentRecordInfoLength);
    return SUCCEED;
}

int DBDataFileDescriptionSlot::getFieldCount()
{
    if(indexes.size() != names.size() ||
       names.size() != types.size() ||
       types.size() != offsets.size())
    {
        return -1;
    }
    return indexes.size();
}

void DBDataFileDescriptionSlot::print()
{
    DBPrint("First data page is at ");
    DBPrintLine(getFirstDataPage());
    DBPrint("First usage slot is at ");
    DBPrintLine(getFirstUsageSlot());
    DBPrint("Last usage slot is at ");
    DBPrintLine(getLastUsageSlot());
    DBPrint("Record length is ");
    DBPrintLine(getRecordLength());
    DBPrint("Record info length is ");
    DBPrintLine(getRecordInfoLength());
    int cnt = getFieldCount();
    DBPrint("Number of fields: ");
    DBPrintLine(cnt);
//    DBPrint("Index: ");
//    DBPrintLine(indexes.size());
//    DBPrint("Name: ");
//    DBPrintLine(names.size());
//    DBPrint("Type: ");
//    DBPrintLine(types.size());
//    DBPrint("Offset: ");
//    DBPrintLine(offsets.size());
    for(int i = 0; i < cnt; i++)
    {
        DBPrint("Field type: ");
        DBPrintLine(DBType::typeName(types[i]));
        DBPrint("Field name: ");
        DBPrintLine(names[i]);
    }
}

int DBDataFileDescriptionSlot::getRecordLength()
{
    return recordLength;
}

int DBDataFileDescriptionSlot::getFirstDataPage()
{
    int re;
    sscanf((char*)firstDataPage, "%d", &re);
    return re;
}

int DBDataFileDescriptionSlot::getFirstUsageSlot()
{
    int re;
    sscanf((char*)firstUsageSlot, "%d", &re);
    return re;
}

int DBDataFileDescriptionSlot::getLastUsageSlot()
{
    int re;
    sscanf((char*)lastUsageSlot, "%d", &re);
    return re;
}

int DBDataFileDescriptionSlot::getRecordInfoLength()
{
    int re;
    sscanf((char*)recordInfoLength, "%d", &re);
    return re;
}

void DBDataFileDescriptionSlot::setFirstDataPage(int n)
{
    sprintf((char*)firstDataPage, "%d", n);
}

void DBDataFileDescriptionSlot::setFirstUsageSlot(int n)
{
    sprintf((char*)firstUsageSlot, "%d", n);
}

void DBDataFileDescriptionSlot::setLastUsageSlot(int n)
{
    sprintf((char*)lastUsageSlot, "%d", n);
}

void DBDataFileDescriptionSlot::setRecordInfoLength(int n)
{
    sprintf((char*)recordInfoLength, "%d", n);
}

int DBDataFileDescriptionSlot::getOffsetOfField(std::string name)
{
    if(indexes.count(name))
    {
        return offsets[indexes[name]];
    }
    return -1;
}

int DBDataFileDescriptionSlot::getTypeOfField(std::string name)
{
    if(indexes.count(name))
    {
        return types[indexes[name]];
    }
    return -1;
}

void DBDataFileDescriptionSlot::write(int fdp, int fus, int lus)
{
    setFirstDataPage(fdp);
    setFirstUsageSlot(fus);
    setLastUsageSlot(lus);
    setRecordInfoLength(currentRecordInfoLength);
    char* cache = (char*)recordInfo;
    if(names.size() != types.size())
    {
        throw ERROR;
    }
    for(int i = 0; i < names.size(); i++)
    {
        int name_length = names[i].size();
        sprintf(cache + RECORD_INFO_TYPE_OFFSET, "%d", types[i]);
        sprintf(cache + RECORD_INFO_NAME_LENGTH_OFFSET, "%d", name_length);
        cache += RECORD_INFO_NAME_OFFSET;
        if(names[i].copy(cache, name_length) != name_length)
        {
            throw ERROR;
        }
        cache += name_length;
    }
}
