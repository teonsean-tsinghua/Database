#include"DBInclude.h"
#include"FileLayer/DBFileInclude.h"
#include"DatabaseLayer/DBDatabaseInclude.h"
using namespace std;

int main()
{
    srand((unsigned)time(NULL));
//    freopen("test.out", "w", stdout);
//    DBIndexFile::test();
    try
    {
        DBDatabase::test();
    }
    catch(Exception& e)
    {
        e.print();
    }
    return 0;
}
