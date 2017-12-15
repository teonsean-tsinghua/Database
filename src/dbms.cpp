#include"database/Database.h"
using namespace std;

int main()
{
    srand((unsigned)time(NULL));
//    freopen("test.out", "w", stdout);
//    IndexFile::test();
    try
    {
        Database::test();
    }
    catch(Exception& e)
    {
        e.print();
    }
    return 0;
}

