#include "FileLayer/DBIndexDataPage.h"
#include "DBInclude.h"
#include "printpage.h"

int main(){
	BufType _cache = new unsigned int[PAGE_SIZE >> 2];
	DBIndexDataPage* idp = new DBIndexDataPage(_cache, 20, 19, MODE_CREATE, 5);
	char* data = new char[9];
	((int*)data)[0] = 12345;
	data[4] = 'a';
	data[5] = 'b';
	data[6] = 'c';
	data[7] = 'd';
	data[8] = 'e';
	char* data2 = new char[9];
	((int*)data2)[0] = 23456;
	data2[4] = 'b';
	data2[5] = 'c';
	data2[6] = 'd';
	data2[7] = 'e';
	data2[8] = 'f';
	char* data3 = new char[5];
	data3[0] = 'x';
	data3[1] = 'e';
	data3[2] = 'm';
	data3[3] = 'b';
	data3[4] = 'o';
	idp -> appendData(data, 9);
	idp -> appendData(data2, 9);
	idp -> insert(data3, 5, 15264, 1);
	std::cout << idp -> getDataCnt() << endl;
	std::cout << idp -> search(data + 4, 5) << endl;
	std::cout << idp -> search(data2 + 4, 5) << endl;
	std::cout << idp -> getMaxSize() << endl;
	std::cout << idp -> getFloatSize() << endl;
	std::cout << idp -> getDataOffsetbyIdx(1) << endl;
	idp -> print();
	idp -> appendData((char*) idp -> getDataHead(), 9);
	idp -> print();
	idp -> forceDelete(1);
	idp -> print();
	return 0;
}