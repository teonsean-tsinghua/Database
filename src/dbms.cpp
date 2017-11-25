#include"DBInclude.h"
#include"FileLayer/DBDataFile.h"
#include"FileLayer/DBIndexFile.h"
using namespace std;

int main()
{
    srand((unsigned)time(NULL));
//    freopen("test.out", "w", stdout);
//    DBDataFile df("");
//    df.test();
    DBIndexFile inf("");
    inf.test();
    return 0;
}
