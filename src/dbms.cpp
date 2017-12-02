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
//table t;
//
//    t.row_num = 5;
//    t.col_num = 3;
//
//    t.col_max_width = (unsigned int *)malloc(sizeof(int) * t.col_num);
//    t.content = (const char ***)malloc(sizeof(const char **) * t.row_num);
//    t.content[0] = (const char **)malloc(sizeof(const char *) * t.col_num);
//    t.content[1] = (const char **)malloc(sizeof(const char *) * t.col_num);
//    t.content[2] = (const char **)malloc(sizeof(const char *) * t.col_num);
//    t.content[3] = (const char **)malloc(sizeof(const char *) * t.col_num);
//    t.content[4] = (const char **)malloc(sizeof(const char *) * t.col_num);
//
//    t.content[0][0] = "AAAAAAAAAAAAAAAAA";
//    t.content[0][1] = "test";
//    t.content[0][2]= "BB";
//
//    t.content[1][0] = "2";
//    t.content[1][1] = "CCCCCCCCCCCCCCCCCCCCCCCCC";
//    t.content[1][2]= "test";
//
//    t.content[2][0] = "3";
//    t.content[2][1] = "EEEEEEEEEEEEEE";
//    t.content[2][2] = "FFFFFFFFFFFFFFFtestFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
//
//    t.content[3][0] = "GGGGGGGGGGGGGGGG";
//    t.content[3][1] = "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
//    t.content[3][2]= "III";
//
//    t.content[4][0] = "GGGGGGGGGGGGGGG";
//    t.content[4][1] = "KKKKKKKKKKKKKKKKKKKK";
//    t.content[4][2] = "LLLLLLLLLLLLLLLLLLLLLLLLL";
//
//    char formated_out[2048];
//    memset(formated_out, 0, 2048);
//    format_table(&t, formated_out);
//
//    printf("%s", formated_out);
//
//    free(t.col_max_width);
//    free(t.content[0]);
//    free(t.content[1]);
//    free(t.content[2]);
//    free(t.content[3]);
//    free(t.content[4]);
//    free(t.content);
    return 0;
}
