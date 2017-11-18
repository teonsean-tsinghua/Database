#include "DBIndexUsageSlot.h"

DBIndexUsageSlot::DBIndexUsageSlot(BufType Cache):DBSlot(cache){}

void DBIndexUsageSlot::setAvailable(int offset, bool available){
    writeBoolToChar((*this)[offset], target);
}

bool DBIndexUsageSlot::isAvailable(int offset)
{
    bool re = false;
    readCharToBool((*this)[offset], &re);
    return re;
}