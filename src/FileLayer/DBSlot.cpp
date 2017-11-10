#include"DBSlot.h"

DBSlot::DBSlot(BufType cache):
    cache(cache)
{

}

int DBSlot::size()
{
    return 0;
}

void DBSlot::print()
{

}

BufType DBSlot::operator[](const int offset) const
{
    return (BufType)((char*)cache + offset);
}
