#include"Slot.h"

Slot::Slot(BufType cache):
    cache(cache)
{

}

BufType Slot::operator[](const int offset) const
{
    return (BufType)((char*)cache + offset);
}
