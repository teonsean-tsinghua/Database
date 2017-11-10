#include"DBUsageSlot.h"

DBUsageSlot::DBUsageSlot(BufType cache):
    DBSlot(cache)
{

}

void DBUsageSlot::setAvailable(int offset, bool available)
{
    if(offset >= 0 && offset < PAGE_SIZE)
    {
        writeBoolToChar((*this)[offset], available);
    }
}

bool DBUsageSlot::isAvailable(int offset)
{
    bool re = false;
    if(offset >= 0 && offset < PAGE_SIZE)
    {
        readCharToBool((*this)[offset], &re);
    }
    return re;
}
