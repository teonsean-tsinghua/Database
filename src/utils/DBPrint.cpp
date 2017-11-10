#include"DBPrint.h"

void DBPrint_ID(char* data)
{
    printf("%x", *(unsigned long long*)data);
    printf("%x", *(unsigned long long*)(data + 8));
}
