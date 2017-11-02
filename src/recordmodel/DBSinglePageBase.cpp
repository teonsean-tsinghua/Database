#include "DBSinglePageBase.h"
#include <stdlib.h>
// malloc.h here in windows
#include <stdio.h>

DBSinglePageBase::DBSinglePageBase(short previous_page_id, short father_page_id, short father_page_offset, short empty_size){
	data = (unsigned char*)malloc(PAGE_SIZE);
	short* info = (short*)&data;
	info[0] = previous_page_id;
	info[1] = father_page_id;
	info[2] = father_page_offset;
	info[3] = empty_size;
}

void DBSinglePageBase::setnextpage(short next_page_id){
	short* info = (short*)&data;
	info[4] = next_page_id;
}

unsigned char* DBSinglePageBase::getdata(){
	return data + 10;
}

bool DBSinglePageBase::setdata(unsigned char* newdata, int len){
	if (len > PAGE_SIZE - 10){
		return false;
	}
	for(short i = 0; i < len; i++)
		data[i + 8] = newdata[i];
	for(short i = len; i < PAGE_SIZE - 10; i++)
		data[i + 8] = 255;
}

void DBSinglePageBase::debug(){
	for(short i = 0; i < PAGE_SIZE; i++)
		printf("%x ", data[i]);
	printf("\n");
}

DBSinglePageBase::~DBSinglePageBase(){
	delete data;
}