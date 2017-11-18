#include "FileLayer/DBIndexDataSlot.h"
#include "DBInclude.h"
#include "printpage.h"

int main(){
	BufType _cache = new unsigned int[PAGE_SIZE >> 2];
	DBIndexDataSlot* ids = new DBIndexDataSlot(_cache, 5);
	ids -> setFatherPageID(15302);
	ids -> setDataCnt(2);
	ids -> setisLeaf(true);
//	_cache[1] = 15302;
//	_cache[2] = 10;
	ids -> writePointer(1, 10);
	ids -> writePointer(0, 2124);
	char* data1 = new char[5];
	data1[0] = 'b';
	data1[1] = 'c';
	data1[2] = 'd';
	data1[3] = 'e';
	data1[4] = 'f';
	ids -> writeData(1, data1, 5);
	ids -> print();
	BufType _cache2 = new unsigned int[PAGE_SIZE >> 2];
	DBIndexDataSlot* ids2 = new DBIndexDataSlot(_cache2, 5);
	ids2 -> setDataCnt(10);
	ids2 -> writeData(0, data1, 5);
	ids2 -> writePointer(0, 2124);
	ids -> appendData(_cache2 + 3, 9);
	ids -> print();
	DBPrintBufPage(_cache, 30);
	return 0;
}