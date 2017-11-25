#include"DBCompare.h"

int compare(void* a, void* b, int type)
{
    int re;
    switch(type)
    {
    case DBType::TEST_DATA_TYPE:
    case DBType::INT:
        {
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
        }
    default:
        re = EQUAL;
        break;
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

