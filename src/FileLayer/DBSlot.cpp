#include"DBSlot.h"

DBSlot::DBSlot(BufType cache, int slotLength, bool parse):
    cache(cache), slotLength(slotLength)
{
    if(slotLength == -1 && parse)
    {
        //slotLength = *(int*)((*this)[SLOT_LENGTH_OFFSET]);
    }
}

int DBSlot::size()
{
    return 0;
}

int DBSlot::getSlotLength()
{
    return slotLength;
}

BufType DBSlot::operator[](const int offset) const
{
    return (BufType)((char*)cache + offset);
}

DBDataFileDescriptionSlot::DBDataFileDescriptionSlot(BufType cache, bool parse):
    DBSlot(cache, 0, parse)
{
    firstDataPage = (*this)[FIRST_DATA_PAGE_OFFSET];
    firstUsageSlot = (*this)[FIRST_USAGE_SLOT_OFFSET];
    lastUsageSlot = (*this)[LAST_USAGE_SLOT_OFFSET];
    recordInfoLength = (*this)[RECORD_INFO_LENGTH_OFFSET];
    recordInfo = (*this)[RECORD_INFO_OFFSET];
    if(parse)
    {
        if(DBParser::parseRecordInfo(indexes, names, types, offsets, (char*)recordInfo, getRecordInfoLength()) != SUCCEED)
        {
            DBPrint("error");
        }
        recordLength = indexes.size() ? offsets[offsets.size() - 1] + DBType::size(types[types.size() - 1]) : 0;
        currentRecordInfoLength = getRecordInfoLength();
    }
    else
    {
        currentRecordInfoLength = 0;
        recordLength = 0;
        indexes.clear();
        names.clear();
        types.clear();
        offsets.clear();
    }
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
    recordLength += DBType::size(type);
    indexes[name] = names.size();
    currentRecordInfoLength += (sizeof(int) + name.size());
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

void DBDataFileDescriptionSlot::printFileDescription()
{
    DBPrint("First data page is at ");
    DBPrint(getFirstDataPage());
    DBPrintLine();
    DBPrint("First usage slot is at ");
    DBPrint(getFirstUsageSlot());
    DBPrintLine();
    DBPrint("Last usage slot is at ");
    DBPrint(getLastUsageSlot());
    DBPrintLine();
    DBPrint("Record length is ");
    DBPrint(getRecordLength());
    DBPrintLine();
    DBPrint("Record info length is ");
    DBPrint(getRecordInfoLength());
    DBPrintLine();
    int cnt = getFieldCount();
    DBPrint("Number of fields: ");
    DBPrint(cnt);
    DBPrintLine();
//    DBPrint("Index: ");
//    DBPrint(indexes.size());
//    DBPrintLine();
//    DBPrint("Name: ");
//    DBPrint(names.size());
//    DBPrintLine();
//    DBPrint("Type: ");
//    DBPrint(types.size());
//    DBPrintLine();
//    DBPrint("Offset: ");
//    DBPrint(offsets.size());
//    DBPrintLine();
    for(int i = 0; i < cnt; i++)
    {
        DBPrint("Field type: ");
        DBPrint(DBType::name(types[i]));
        DBPrintLine();
        DBPrint("Field name: ");
        DBPrint(names[i]);
        DBPrintLine();
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
    DBParser::writeRecordInfo(names, types, (char*)recordInfo, recordLength);
}

DBPageInfoSlot::DBPageInfoSlot(BufType cache, bool parse):
    DBSlot(cache, parse)
{
    pageType = (*this)[PAGE_TYPE_OFFSET];
    firstAvailableByte = (*this)[FIRST_AVAILABLE_BYTE_OFFSET];
    lengthFixed = (*this)[LENGTH_FIXED_OFFSET];
    nextSamePage = (*this)[NEXT_SAME_PAGE_OFFSET];
}

int DBPageInfoSlot::size()
{
    return sizeof(int) * 3 + sizeof(bool);
}

void DBPageInfoSlot::write()
{
    sprintf((char*)cache, "%d%d%d%d", 0, 0, 0, 0);
}

int DBPageInfoSlot::getPageType()
{
    int re;
    sscanf((char*)pageType, "%d", &re);
    return re;
}

int DBPageInfoSlot::getFirstAvailableByte()
{
    int re;
    sscanf((char*)firstAvailableByte, "%d", &re);
    return re;
}

bool DBPageInfoSlot::isLengthFixed()
{
    int re;
    sscanf((char*)lengthFixed, "%d", &re);
    return re;
}

int DBPageInfoSlot::getNextSamePage()
{
    int re;
    sscanf((char*)nextSamePage, "%d", &re);
    return re;
}

DBRecordSlot::DBRecordSlot(BufType cache, int slotLength):
    DBSlot(cache, slotLength)
{

}
