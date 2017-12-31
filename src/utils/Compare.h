#ifndef COMPARE_H_INCLUDED
#define COMPARE_H_INCLUDED

#include"Type.h"

bool larger(void* a, void* b, int type);

bool smaller(void* a, void* b, int type);

bool Equal(void* a, void* b, int type);

bool largerOrEqual(void* a, void* b, int type);

bool smallerOrEqual(void* a, void* b, int type);

#endif // COMPARE_H_INCLUDED
