#include"DBUsageSlot.h"

DBUsageSlot::DBUsageSlot(BufType cache):
    DBSlot(cache)
{

}

void DBUsageSlot::setAvailable(int offset, bool available)
{
    writeBoolToChar((*this)[offset], available);
}

bool DBUsageSlot::isAvailable(int offset)
{
    bool re = false;
    readCharToBool((*this)[offset], &re);
    return re;
}
