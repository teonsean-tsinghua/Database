#include"database/Database.h"
using namespace std;

void test()
{
//	FileIOModel::test();
//	DataFile::test();
//	Database::test();
    UsagePage::test();
    char buf[8192];
    NodePage<int> t(buf, 1, 1, 1, 1, 1, 1);
    NodePage<int>::test();
}

int main()
{
    srand((unsigned)time(NULL));
    FileIOModel::getInstance()->setRoot("/home/teon/Documents/Database");
//    freopen("test.out", "w", stdout);
//    IndexFile::test();
    try
    {
        test();
    }
    catch(Exception& e)
    {
    	Database::getInstance()->init();
        e.print();
    }
    return 0;
}

