#include"CacheIOModel.h"

static char buf[33];

int readInt(const char* cache)
{
    return *(int*)cache;
}

void writeInt(char* cache, int n)
{
    *(int*)cache = n;
}

float readFloat(const char* cache)
{
    return *(float*)cache;
}

void writeFloat(char* cache, float n)
{
    *(float*)cache = n;
}

std::string readString(const char* cache, int len)
{
    std::string str;
    str = str.assign(cache, len);
    return str;
}

void writeString(char* cache, std::string& str, int len)
{
    str.copy(cache, len);
}

bool readBool(const char* cache)
{
    return *cache == 1;
}

void writeBool(char* cache, bool b)
{
    *cache = b ? 1 : 0;
}

void copyData(const char* src, char* dest, int len)
{
    memcpy(dest, src, len);
}

std::string read_id(const char* cache)
{
    sprintf(buf, "%08X%08X%08X%04X%04X", (unsigned int)((*(unsigned long long*)cache) >> 32), (unsigned int)*(unsigned long long*)cache,
            *(unsigned int*)((char*)cache + 8), *(unsigned short*)((char*)cache + 12), *(unsigned short*)((char*)cache + 14));
    return std::string(buf);
}

void write_id(char* cache)
{
    *(unsigned long long*)cache = time(0);
    *(unsigned int*)((char*)cache + 8) = (unsigned int)(unsigned long long)cache;
    *(unsigned short*)((char*)cache + 12) = (unsigned short)getpid();
    *(unsigned short*)((char*)cache + 14) = (unsigned short)(rand());
}
