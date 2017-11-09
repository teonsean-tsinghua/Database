#include"DBSlot.h"

DBSlot::DBSlot(BufType cache, int slotLength):
    cache(cache), slotLength(slotLength)
{
    if(slotLength == -1)
    {
        //slotLength = *(int*)((*this)[SLOT_LENGTH_OFFSET]);
    }
}

int DBSlot::getSlotLength()
{
    return slotLength;
}

BufType DBSlot::operator[](const int offset) const
{
    return (BufType)((char*)cache + offset);
}

DBDataFileDescriptionSlot::DBDataFileDescriptionSlot(BufType cache):
    DBSlot(cache, 0)
{
    firstDataPage = (*this)[FIRST_DATA_PAGE_OFFSET];
    firstUsageSlot = (*this)[FIRST_USAGE_SLOT_OFFSET];
    lastUsageSlot = (*this)[LAST_USAGE_SLOT_OFFSET];
    recordInfoLength = (*this)[RECORD_INFO_LENGTH_OFFSET];
    recordInfo = (*this)[RECORD_INFO_OFFSET];
    DBParser::parseRecordInfo(indexes, names, types, offsets, (char*)recordInfo, getRecordInfoLength());
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

DBPageInfoSlot::DBPageInfoSlot(BufType cache, int slotLength):
    DBSlot(cache, slotLength)
{
    pageType = (*this)[PAGE_TYPE_OFFSET];
    firstAvailableByte = (*this)[FIRST_AVAILABLE_BYTE_OFFSET];
    lengthFixed = (*this)[LENGTH_FIXED_OFFSET];
    nextSamePage = (*this)[NEXT_SAME_PAGE_OFFSET];
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
