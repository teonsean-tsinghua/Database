#include"DBInclude.h"
#include"FileLayer/DBFileInclude.h"
#include"DatabaseLayer/DBDatabaseInclude.h"
using namespace std;

int main()
{
    srand((unsigned)time(NULL));
//    freopen("test.out", "w", stdout);
//    DBDataFile::test();
//    DBIndexFile::test();
    DBDatabase::test();
    return 0;
}
