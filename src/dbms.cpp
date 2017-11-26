#include"DBInclude.h"
#include"FileLayer/DBFileInclude.h"
#include"DatabaseLayer/DBDatabaseInclude.h"
using namespace std;

int main()
{
    srand((unsigned)time(NULL));
//    freopen("test.out", "w", stdout);
    const char* sFile="test.sql";
    FILE* fp=fopen(sFile, "r");
    if(fp==NULL)
    {
        printf("cannot open %s\n", sFile);
        return -1;
    }
    extern FILE* yyin;
    yyin=fp;

    printf("-----begin parsing %s\n", sFile);
    yyparse();
    puts("-----end parsing");

    fclose(fp);
    return 0;
}
