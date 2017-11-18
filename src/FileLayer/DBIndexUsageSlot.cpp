#include"DBIndexUsageSlot.h"

DBIndexUsageSlot::DBIndexUsageSlot(BufType cache):
    DBSlot(cache)
{

}

void DBIndexUsageSlot::setAvailable(int offset, bool available)
{
    writeBoolToChar((*this)[offset], available);
}

bool DBIndexUsageSlot::isAvailable(int offset)
{
    bool re = false;
    readCharToBool((*this)[offset], &re);
    return re;
}
