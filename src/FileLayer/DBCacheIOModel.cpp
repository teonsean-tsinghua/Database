#include"DBCacheIOModel.h"

void readInt(BufType cache, int* n)
{
    *n = *(int*)cache;
}

void writeInt(BufType cache, int n)
{
    *(int*)cache = n;
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
    strncpy(data, (char*)cache, len);
}

void writeData(BufType cache, char* data, int len)
{
    strncpy((char*)cache, data, len);
}
