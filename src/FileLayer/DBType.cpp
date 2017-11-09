#include"DBType.h"

const int DBType::size_[] = {sizeof(int)};

const char* const DBType::name_[] = {"int"};

int DBType::size(int type)
{
    if(type < FIRST || type > LAST)
    {
        return -1;
    }
    return size_[type];
}

const char* DBType::name(int type)
{
    if(type < FIRST || type > LAST)
    {
        return NULL;
    }
    return name_[type];
}
