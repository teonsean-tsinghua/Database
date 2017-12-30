#include"database/Database.h"
using namespace std;

void test()
{
//	FileIOModel::test();
	Database::test();
//    UsagePage::test();
//    NodePage<int> test(new char[8192], 1, 1, 1, 1, 1, 1);
//    NodePage<int>::test();
//	DataFile::test();
//	IndexFile<int> foo;
//    IndexFile<int>::test();
}

int main()
{
    srand((unsigned)time(NULL));
    FileIOModel::getInstance()->setRoot("/home/teon/Documents/Database");
//    freopen("test.out", "w", stdout);
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

