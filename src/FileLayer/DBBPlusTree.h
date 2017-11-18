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

	void solveUnderFlow(char* data, int len, int pageID);

	void solveOverFlow(char* data, int len, int pageID);

	int getBrother(char* data, int len, int pageID);

	void mergePage(int leftPageID, int rightPageID, int fatherPageID, char* data, int len);

	void rotateLeft(int leftPageID, int rightPageID, int fatherPageID, char* data, int len);

	void rotateRight(int leftPageID, int rightPageID, int fatherPageID, char* data, int len);
};

#endif