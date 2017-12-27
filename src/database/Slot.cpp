#include"Slot.h"

Slot::Slot(char* cache):
    cache(cache)
{

}

char* Slot::operator[](const int offset) const
{
    return (cache + offset);
}
