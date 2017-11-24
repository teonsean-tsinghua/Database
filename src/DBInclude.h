#ifndef DBINCLUDE_H_INCLUDED
#define DBINCLUDE_H_INCLUDED

#include"utils/DBPrint.h"
#include"utils/DBType.h"
#include"utils/DBRecordInfo.h"
#include<ctime>
#include<unistd.h>
#include<string>
#include<iostream>
#include<cstring>
#include<map>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<set>

const static int SUCCEED = 0;
const static int ERROR = SUCCEED - 1;
const static int FIELD_ALREADY_EXIST = ERROR - 1;
const static int EXCEED_PAGE_LIMIT = FIELD_ALREADY_EXIST - 1;
const static int FILE_OR_DIRECTORY_DOES_NOT_EXIST = EXCEED_PAGE_LIMIT - 1;
const static int FIELD_NOT_EXIST = FILE_OR_DIRECTORY_DOES_NOT_EXIST - 1;
const static int FIELD_IS_ALREADY_PRIMARY_KEY = FIELD_NOT_EXIST - 1;
const static int EMPTY_FIELD_NAME = FIELD_IS_ALREADY_PRIMARY_KEY - 1;
const static int DATA_PAGE_FULL = EMPTY_FIELD_NAME - 1;
const static int EQUAL_RECORD = DATA_PAGE_FULL - 1;
const static int NON_EQUAL_RECORD = EQUAL_RECORD - 1;
const static int A_FILE_ALREADY_OPENED = NON_EQUAL_RECORD - 1;
const static int FILE_NOT_OPENED = A_FILE_ALREADY_OPENED - 1;

const static int MODE_CREATE = 101;
const static int MODE_PARSE = 202;

const static int UNNULLABLE = 0;
const static int EXTRA_FIELD = 1;
const static int EDIT__ID = 2;

#endif // DBINCLUDE_H_INCLUDED
