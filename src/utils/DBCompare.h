#ifndef DBCOMPARE_H_INCLUDED
#define DBCOMPARE_H_INCLUDED

#include"DBType.h"

const static int EQUAL = 0;
const static int LARGER = 1;
const static int SMALLER = -1;

int compare(void* a, void* b, int type);

bool larger(void* a, void* b, int type);

bool smaller(void* a, void* b, int type);

bool equal(void* a, void* b, int type);

bool largerOrEqual(void* a, void* b, int type);

bool smallerOrEqual(void* a, void* b, int type);

#endif // DBCOMPARE_H_INCLUDED
