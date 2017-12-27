#include"CacheIOModel.h"

void readInt(char* cache, int* n)
{
    *n = *(int*)cache;
}

void writeInt(char* cache, int n)
{
    *(int*)cache = n;
}

void readString(char* cache, std::string& str, int len)
{
    str = str.assign(cache, len);
}

void writeString(char* cache, std::string& str, int len)
{
    str.copy(cache, len);
}

void readCharToBool(char* cache, bool* b)
{
    *b = (bool)(*(char*)cache);
}

void writeBoolToChar(char* cache, bool b)
{
    *(char*)cache = (char)(b ? 1 : 0);
}

void readData(char* cache, char* data, int len)
{
    memcpy(data, (char*)cache, len);
}

void writeData(char* cache, char* data, int len)
{
    memcpy((char*)cache, data, len);
}

void copyData(char* src, char* dest, int len)
{
    memcpy((char*)dest, (char*)src, len);
}

void read_id(char* cache, char* data)
{
    *(unsigned long long*)data = *(unsigned long long*)cache;
    *(unsigned int*)(data + 8) = *(unsigned int*)((char*)cache + 8);
    *(unsigned short*)(data + 12) = *(unsigned short*)((char*)cache + 12);
    *(unsigned short*)(data + 14) = *(unsigned short*)((char*)cache + 14);
}

void write_id(char* cache)
{
    *(unsigned long long*)cache = time(0);
    *(unsigned int*)((char*)cache + 8) = (unsigned int)(unsigned long long)cache;
    *(unsigned short*)((char*)cache + 12) = (unsigned short)getpid();
    *(unsigned short*)((char*)cache + 14) = (unsigned short)(rand());
}

