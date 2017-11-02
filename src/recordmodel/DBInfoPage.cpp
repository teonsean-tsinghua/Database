#include "DBInfoPage.h"
#include <stdlib.h>

DBInfoPage::DBInfoPage(){
	data = (unsigned char*) malloc(PAGE_SIZE);
	data[0] = 1;
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

unsigned short DBInfoPage::allocatenewpage(){
	for(unsigned short i = 1; i !=0; i++)
		if (this -> getbit(i) == false){
			this -> setbit(i, 1);
			return i;
		}
	return 0;
}

DBInfoPage::~DBInfoPage(){
	delete data;
}