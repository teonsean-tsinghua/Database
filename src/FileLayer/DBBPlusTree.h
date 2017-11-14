#ifndef DBBPLUSTREE_H_INCLUDED
#define DBBPLUSTREE_H_INCLUDED

#include "DBIndexDataPage.h"
#include "DBIndexFile.h"

class DBBPlusTree{
private:
	DBIndexDataPage* idp;
	DBIndexFile* idf;
	int currentPageID;
	int dataLen;
public:
	DBBPlusTree(const char* indexname, int _dataLen);

	void insert(char* data, int len);

	void deleteData(char* data, int len);

	int search(char* data, int len);

	void solveUnderFlow();

	void solveOverFlow();
};

#endif