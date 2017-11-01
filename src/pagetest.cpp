#include <iostream>
#include <cstdlib>
#include "recordmodel/DBPageManager.h"
using namespace std;

int main(){
	DBPageManager* page = new DBPageManager(0x0001,0x0003,0x0003);
	unsigned char data[PAGE_SIZE - 8];
	for(short i = 0; i < PAGE_SIZE - 8; i++)
		data[i] = i;
	page -> setdata(data, PAGE_SIZE - 8);
	page -> debug();
	return 0;
}