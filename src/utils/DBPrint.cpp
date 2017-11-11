#include"DBPrint.h"

void DBPrint_ID(char* data)
{
    printf("%016x", *(unsigned long long*)data);
    printf("%08x", *(unsigned int*)(data + 8));
    printf("%08x", *(unsigned int*)(data + 12));
    printf("%04x", *(unsigned short*)(data + 16));
    printf("%04x", *(unsigned short*)(data + 18));
}
