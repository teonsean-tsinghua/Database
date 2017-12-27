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

void write_id(char* cache)
{
    *(unsigned long long*)cache = time(0);
    *(unsigned int*)((char*)cache + 8) = rand() * rand();
    *(unsigned int*)((char*)cache + 12) = (unsigned int)(unsigned long long)cache;
    *(unsigned short*)((char*)cache + 16) = (unsigned short)getpid();
    *(unsigned short*)((char*)cache + 18) = (unsigned short)(rand());
}
