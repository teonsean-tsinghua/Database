#ifndef DBCOMPARE_H_INCLUDED
#define DBCOMPARE_H_INCLUDED

#include"DBType.h"

const static int EQUAL = 0;
const static int LARGER = 1;
const static int SMALLER = -1;

int compare(void* a, void* b, int type)
{
    int re;
    switch(type)
    {
    case DBType::INT:
        int aa = *(int*)a;
        int bb = *(int*)b;
        if(aa > bb)
        {
            re = LARGER;
        }
        else if(aa == bb)
        {
            re = EQUAL;
        }
        else
        {
            re = SMALLER;
        }
        break;
    default:
        re = EQUAL;
    }
    return re;
}

bool larger(void* a, void* b, int type)
{
    return compare(a, b, type) == LARGER;
}

bool smaller(void* a, void* b, int type)
{
    return compare(a, b, type) == SMALLER;
}

bool equal(void* a, void* b, int type)
{
    return compare(a, b, type) == EQUAL;
}

bool largerOrEqual(void* a, void* b, int type)
{
    return compare(a, b, type) >= EQUAL;
}

bool smallerOrEqual(void* a, void* b, int type)
{
    return compare(a, b, type) <= EQUAL;
}

#endif // DBCOMPARE_H_INCLUDED
