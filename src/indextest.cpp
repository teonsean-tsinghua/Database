#include "FileLayer/DBIndexDataPage.h"
#include "FileLayer/DBIndexFile.h"
#include "DBInclude.h"
#include "printpage.h"

int main(){
	/*
	BufType _cache = new unsigned int[PAGE_SIZE >> 2];
	DBIndexDataPage* idp = new DBIndexDataPage(_cache, 20, 19, MODE_CREATE, 5);
	char* data = new char[13];
	((int*)data)[0] = 12345;
	((int*)data)[1] = 6;
	data[8] = 'a';
	data[9] = 'b';
	data[10] = 'c';
	data[11] = 'd';
	data[12] = 'e';
	char* data2 = new char[13];
	((int*)data2)[0] = 23456;
	((int*)data2)[1] = 8;
	data2[8] = 'b';
	data2[9] = 'c';
	data2[10] = 'd';
	data2[11] = 'e';
	data2[12] = 'f';
	char* data3 = new char[5];
	data3[0] = 'x';
	data3[1] = 'e';
	data3[2] = 'm';
	data3[3] = 'b';
	data3[4] = 'o';
	idp -> appendData(data, 13);
	idp -> appendData(data2, 13);
	idp -> print();
	idp -> insert(data3, 5, 15264, 1, 12);
	idp -> print();
	idp -> appendData((char*) idp -> getDataHead(), 13);
	idp -> print();
	idp -> forceDelete(1);
	idp -> print();
	/*/
	DBIndexFile* idf = new DBIndexFile("");
	idf -> createFile("indextest.db");
	idf -> openFile("indextest.db");
	for(int i = 0; i < 237; i++)
		idf -> allocateNewIndexPage();
	return 0;
}