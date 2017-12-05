#include"DBSlot.h"

DBSlot::DBSlot(BufType cache):
    cache(cache)
{

}

BufType DBSlot::operator[](const int offset) const
{
    return (BufType)((char*)cache + offset);
}
