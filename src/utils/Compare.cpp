#include"Compare.h"

int compare(void* a, void* b, int type, int length)
{
    int re;
    switch(type)
    {
    case Type::INT:
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

bool larger(void* a, void* b, int type, int length)
{
    return compare(a, b, type, length) == LARGER;
}

bool smaller(void* a, void* b, int type, int length)
{
    return compare(a, b, type, length) == SMALLER;
}

bool Equal(void* a, void* b, int type, int length)
{
    return compare(a, b, type, length) == EQUAL;
}

bool largerOrEqual(void* a, void* b, int type, int length)
{
    return compare(a, b, type, length) >= EQUAL;
}

bool smallerOrEqual(void* a, void* b, int type, int length)
{
    return compare(a, b, type, length) <= EQUAL;
}

