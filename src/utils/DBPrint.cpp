#include"DBPrint.h"

DBPrint* DBPrint::instance = new DBPrint();

DBPrint::DBPrint()
{

}

DBPrint& DBPrint::print_ID(char* data)
{
    printf("%016llx", *(unsigned long long*)data);
    printf("%08x", *(unsigned int*)(data + 8));
    printf("%08x", *(unsigned int*)(data + 12));
    printf("%04x", *(unsigned short*)(data + 16));
    printf("%04x", *(unsigned short*)(data + 18));
    return *instance;
}
