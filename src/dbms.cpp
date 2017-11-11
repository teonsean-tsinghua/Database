#include"DBInclude.h"
#include"FileLayer/DBDataFile.h"
using namespace std;

int main()
{
    DBDataFile df("");
//    freopen("test.out", "w", stdout);
    df.createFile("test.db");
    df.openFile("test.db");
//    df.setPrimaryKey("_id");
//    df.setPrimaryKey("test1");
    int data = 1;
    int data2 = 99999;
    int data3 = -99999;
    int data4 = 10;
//    int i = 0;
//    do
//    {
//        df.printFileDescription();
//        DBLogLine("=========================================");
//        sprintf(name, "test%d", i++);
//    }while(df.addField(name, DBType::INT, true) == SUCCEED);
    df.addField("test1", DBType::INT, false);
    df.addField("test2", DBType::INT, false);
    df.addField("test3", DBType::INT, true);
    map<string, void*> f, f2, f3;
    f["test1"] = &data;
    f["test2"] = &data2;
    f["test3"] = &data3;
    for(int i = 0; i < 1000; i++)
        df.insertRecord(f);
    f2["test2"] = &data4;
    df.update(f, f2);
    f3["test2"] = &data4;
//    df.printAllRecords();
    df.closeFile();
    df.openFile("test.db");
//    df.printFileDescription();
//    df.printAllRecords();
    set<map<string, void*>*> re;
    df.findEqual(f3, re);
    return 0;
}
