#ifndef COMPARE_H_INCLUDED
#define COMPARE_H_INCLUDED

#include"Type.h"

bool larger(const void* a, const void* b, int type);

bool smaller(const void* a, const void* b, int type);

bool Equal(const void* a, const void* b, int type);

bool largerOrEqual(const void* a, const void* b, int type);

bool smallerOrEqual(const void* a, const void* b, int type);

#endif // COMPARE_H_INCLUDED
