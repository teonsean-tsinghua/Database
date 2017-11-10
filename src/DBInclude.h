#ifndef DBINCLUDE_H_INCLUDED
#define DBINCLUDE_H_INCLUDED

#include"utils/DBPrint.h"
#include<string>
#include<iostream>
#include<cstring>
#include<map>
#include<vector>
#include<cstdio>

const static int SUCCEED = 0;
const static int ERROR = SUCCEED - 1;
const static int FIELD_ALREADY_EXIST = ERROR - 1;
const static int EXCEED_PAGE_LIMIT = FIELD_ALREADY_EXIST - 1;
const static int FILE_OR_DIRECTORY_DOES_NOT_EXIST = EXCEED_PAGE_LIMIT - 1;

#endif // DBINCLUDE_H_INCLUDED
