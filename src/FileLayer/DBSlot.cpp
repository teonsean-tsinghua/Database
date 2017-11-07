#include"DBSlot.h"

DBSlot::DBSlot(BufType cache, int slotLength):
    cache(cache), slotLength(slotLength)
{
    if(slotLength == -1)
    {
        slotLength = *(int*)((*this)[SLOT_LENGTH_OFFSET]);
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

DBPageInfoSlot::DBPageInfoSlot(BufType cache, int slotLength):
    DBSlot(cache, slotLength)
{
    pageType = (short*)((*this)[PAGE_TYPE_OFFSET]);
    firstAvailableByte = (short*)((*this)[FIRST_AVAILABLE_BYTE_OFFSET]);
    lengthFixed = (bool*)((*this)[LENGTH_FIXED_OFFSET]);
    nextSamePage = (int*)((*this)[NEXT_SAME_PAGE_OFFSET]);
}

short DBPageInfoSlot::getPageType()
{
    return *pageType;
}

short DBPageInfoSlot::getFirstAvailableByte()
{
    return *firstAvailableByte;
}

bool DBPageInfoSlot::isLengthFixed()
{
    return *lengthFixed;
}

int DBPageInfoSlot::getNextSamePage()
{
    return *nextSamePage;
}

DBRecordSlot::DBRecordSlot(BufType cache, int slotLength):
    DBSlot(cache, slotLength)
{

}
