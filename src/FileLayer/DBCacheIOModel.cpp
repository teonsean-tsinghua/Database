#include"DBCacheIOModel.h"

void writeInt(BufType cache, int n)
{
    *(int*)cache = n;
}

void readInt(BufType cache, int* n)
{
    *n = *(int*)cache;
}

void readString(char* cache, std::string& str, int len)
{
    str = str.assign(cache, len);
}

void writeString(char* cache, std::string& str, int len)
{
    if(str.copy(cache, len) != len)
    {
        throw ERROR;
    }
}
