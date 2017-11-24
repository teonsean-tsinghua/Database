#include "FileLayer/DBIndexDataPage.h"
#include "FileLayer/DBIndexFile.h"
#include "FileLayer/DBBPlusTree.h"
#include "DBInclude.h"
#include "printpage.h"

int main(){
	/*
	DBIndexFile* idf = new DBIndexFile("", 5);
//	idf -> createFile("indextest.db");
	idf -> openFile("indextest.db");
	for(int i = 0; i < 1; i++){
		idf -> allocateNewIndexPage();
	}
	DBIndexDataPage* idp = idf -> openIndexDataPage(1);
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
	data2[8] = 'z';
	data2[9] = 'z';
	data2[10] = 'z';
	data2[11] = 'z';
	data2[12] = 'z';
	char* data3 = new char[5];
	data3[0] = 'x';
	data3[1] = 'e';
	data3[2] = 'm';
	data3[3] = 'b';
	data3[4] = 'o';
	for(int i = 0; i < 10; i++)
		idp -> appendData(data, 13);
	for(int i = 0; i < 10; i++)
		idp -> appendData(data2, 13);
	insert(data3, 5, 123, 456);
	idp -> print();
	idf -> closeFile();
	*/
	char* loc = new char[12];
	loc = "indextest.db";
	DBBPlusTree* bpt = new DBBPlusTree(loc, 5);
	bpt -> createFile();
	for(int i = 0; i < 10; i++)
		bpt -> insert(loc, 5, i + 100, i);
	bpt -> print();
//	bpt -> closeFile();
	return 0;
}