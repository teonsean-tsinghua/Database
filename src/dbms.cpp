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
    while(true)
    {
//        cout << ">>";
//        string str;
//        getline(cin,str);
//        if(str=="q" || str=="quit" || str=="QUIT")
//        {
//            break;
//        }
        try
        {
//            FILE* fp=fopen(".___input_processor", "w");
//            fwrite(str.c_str(), 1, str.size(), fp);
//            fclose(fp);
//            fp=fopen(".___input_processor", "r");
//            extern FILE* yyin;
//            yyin=fp;
//            yyparse();
//            fclose(fp);
          test();
        }
        catch(Exception& e)
        {
            Database::getInstance()->init();
            e.print();
        }
    }
    return 0;
}

