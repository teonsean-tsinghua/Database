#include"DBPrint.h"

void DBPrint_ID(char* data)
{
    printf("%llx", *(unsigned long long*)data);
    printf("%llx", *(unsigned long long*)(data + 8));
}
