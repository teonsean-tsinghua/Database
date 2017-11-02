#include "DBInfoPage.h"
#include <stdlib.h>
// Change to malloc.h for windows
#include <stdio.h>
#include <iostream>
using namespace std;

DBInfoPage::DBInfoPage(){
	data = (unsigned char*) malloc(PAGE_SIZE);
}

void DBInfoPage::setbit(unsigned short bit_id, bool val){
	unsigned short byte_id, offset;
	byte_id = bit_id >> 3;
	offset = bit_id ^ (byte_id << 3);
	unsigned char _tmp = data[byte_id];
	_tmp = _tmp - (((_tmp >> offset) % 2) << offset);
	_tmp = _tmp + ((unsigned)val << offset);
	data[byte_id] = _tmp;
}

bool DBInfoPage::getbit(unsigned short bit_id){
	unsigned short byte_id, offset;
	byte_id = bit_id >> 3;
	offset = bit_id ^ (byte_id << 3);
	unsigned char _tmp = data[byte_id];
	return (bool) (_tmp >> offset) % 2;
}

void DBInfoPage::debug(){
	for(unsigned short i = 0; i < PAGE_SIZE; i++)
	printf("%x ", data[i]);
	printf("\n");
}

DBInfoPage::~DBInfoPage(){
	delete data;
}