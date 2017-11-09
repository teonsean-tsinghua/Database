#include"DBSlot.h"

DBSlot::DBSlot(BufType cache, int slotLength, int parse):
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

void DBSlot::print()
{

}

int DBSlot::getSlotLength()
{
    return slotLength;
}

BufType DBSlot::operator[](const int offset) const
{
    return (BufType)((char*)cache + offset);
}
