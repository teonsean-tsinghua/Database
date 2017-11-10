#include"DBType.h"
#include<cstdio>

const int DBType::typeSize_[] = {16, sizeof(int)};

const char* const DBType::typeName_[] = {"_id", "int"};

const char* const DBType::pageName_[] = {"Data file description page",
                                         "Data page",
                                         "Index page",
                                         "Data page"};

int DBType::typeSize(int type)
{
    if(type < FIRST_TYPE || type > LAST_TYPE)
    {
        return -1;
    }
    return typeSize_[type];
}

const char* DBType::typeName(int type)
{
    if(type < FIRST_TYPE || type > LAST_TYPE)
    {
        return NULL;
    }
    return typeName_[type];
}

const char* DBType::pageName(int type)
{
    if(type < FIRST_PAGE_TYPE || type > LAST_PAGE_TYPE)
    {
        return NULL;
    }
    return pageName_[type];
}

