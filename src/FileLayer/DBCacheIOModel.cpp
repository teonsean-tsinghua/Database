#include"DBCacheIOModel.h"

void readInt(BufType cache, int* n)
{
    *n = *(int*)cache;
}

void readUnsignedInt(BufType cache, unsigned int* n){
    *n = *(unsigned int*) cache;
}

void writeInt(BufType cache, int n)
{
    *(int*)cache = n;
}

void writeUnsignedInt(BufType cache, unsigned int n){
    *(unsigned int*) cache = n;
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

void readCharToBool(BufType cache, bool* b)
{
    *b = (bool)(*(char*)cache);
}

void writeBoolToChar(BufType cache, bool b)
{
    *(char*)cache = (char)(b ? 1 : 0);
}

void readData(BufType cache, char* data, int len)
{
    memcpy(data, (char*)cache, len);
}

void writeData(BufType cache, char* data, int len)
{
    memcpy((char*)cache, data, len);
}

void copyData(BufType src, BufType dest, int len)
{
    memcpy((char*)dest, (char*)src, len);
}

void read_id(BufType cache, char* data)
{
    *(unsigned long long*)data = *(unsigned long long*)cache;
    *(unsigned int*)(data + 8) = *(unsigned int*)((char*)cache + 8);
    *(unsigned int*)(data + 12) = *(unsigned int*)((char*)cache + 12);
    *(unsigned short*)(data + 16) = *(unsigned short*)((char*)cache + 16);
    *(unsigned short*)(data + 18) = *(unsigned short*)((char*)cache + 18);
}

void write_id(BufType cache)
{
    *(unsigned long long*)cache = time(0);
    *(unsigned int*)((char*)cache + 8) = rand() * rand();
    *(unsigned int*)((char*)cache + 12) = (unsigned int)(unsigned long long)cache;
    *(unsigned short*)((char*)cache + 16) = (unsigned short)getpid();
    *(unsigned short*)((char*)cache + 18) = (unsigned short)(rand());
}

