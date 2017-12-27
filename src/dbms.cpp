#include"database/Database.h"
using namespace std;

void test()
{
//	FileIOModel::test();
//	DataFile::test();
	Database::test();
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

