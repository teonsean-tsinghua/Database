#ifndef DBINCLUDE_H_INCLUDED
#define DBINCLUDE_H_INCLUDED

#include"utils/DBPrint.h"
#include"utils/DBType.h"
#include"utils/DBRecordInfo.h"
#include"utils/DBCompare.h"
#include"utils/DBException.h"
#include<ctime>
#include<unistd.h>
#include<string>
#include<iostream>
#include<sstream>
#include<cstring>
#include<map>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<set>
#include<list>
#include<cassert>

const static int MODE_CREATE = 101;
const static int MODE_PARSE = 202;

const static int UNNULLABLE = 0;
const static int EXTRA_FIELD = 1;
const static int EDIT__ID = 2;

#endif // DBINCLUDE_H_INCLUDED
