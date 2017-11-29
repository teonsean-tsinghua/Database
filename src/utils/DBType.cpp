#include"DBType.h"
#include<cstdio>

const int DBType::typeSize_[] = {20, sizeof(int), sizeof(float), 8, 0};

const char* const DBType::typeName_[] = {"_ID", "INT", "FLOAT", "DATE", "VARCHAR"};

const char* const DBType::pageName_[] = {"Data file description page",
                                         "Data page",
                                         "Usage page",
                                         "Index file description page",
                                         "Index internal page",
                                         "Index leaf page"};

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

