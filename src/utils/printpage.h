#ifndef PRINTPAGE_H_INCLUDED
#define PRINTPAGE_H_INCLUDED
#include "pagedef.h"
#include <cstdio>

void DBPrintPage(unsigned char* start){
	printf("%x", start[0]);
	for(int i = 1; i < PAGE_SIZE; i++)
		printf(" %x", start[i]);
	printf("\n");
}

#endif