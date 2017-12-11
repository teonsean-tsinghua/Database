#ifndef COMPARE_H_INCLUDED
#define COMPARE_H_INCLUDED

#include"Type.h"

const static int EQUAL = 0;
const static int LARGER = 1;
const static int SMALLER = -1;

int compare(void* a, void* b, int type, int length);

bool larger(void* a, void* b, int type, int length);

bool smaller(void* a, void* b, int type, int length);

bool Equal(void* a, void* b, int type, int length);

bool largerOrEqual(void* a, void* b, int type, int length);

bool smallerOrEqual(void* a, void* b, int type, int length);

#endif // COMPARE_H_INCLUDED
