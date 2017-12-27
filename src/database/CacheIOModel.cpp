#include"CacheIOModel.h"

int readInt(char* cache)
{
    return *(int*)cache;
}

void writeInt(char* cache, int n)
{
    *(int*)cache = n;
}

std::string readString(char* cache, int len)
{
    std::string str;
    str = str.assign(cache, len);
    return str;
}

void writeString(char* cache, std::string& str, int len)
{
    str.copy(cache, len);
}

bool readBool(char* cache)
{
    return *cache == 1;
}

void writeBool(char* cache, bool b)
{
    *cache = b ? 1 : 0;
}

void copyData(char* src, char* dest, int len)
{
    memcpy((char*)dest, (char*)src, len);
}

