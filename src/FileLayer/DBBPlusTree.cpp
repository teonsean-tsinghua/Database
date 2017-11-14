#include "DBBPlusTree.h"

DBBPlusTree::DBBPlusTree(const char* indexname, int _dataLen){
	idf = new DBIndexFile(indexname, _dataLen);
	dataLen = _dataLen;
}

void DBBPlusTree::insert(char* data, int len){
	while (!idp -> getisLeaf()){
		idp = idf -> openIndexDataPage((idp -> searchIdx(data, len)));
	}
	idp -> insert(data, len, idp -> search(data, len), idp -> searchIdx(data, len));
}

void DBBPlusTree::deleteData(char* data, int len){
	while (!idp -> getisLeaf()){
		idp = idf -> openIndexDataPage(idp -> searchIdx(data, len));
	}
	idp -> deleteByIdx(idp -> searchIdx(data, len));
}

int DBBPlusTree::search(char* data, int len){
	while(!idp -> getisLeaf()){
		idp = idf -> openIndexDataPage(idp -> searchIdx(data, len));
	}
	return idp -> searchIdx(data, len);
}

void DBBPlusTree::solveUnderFlow(){
	idf -> setAvailableOfIndexPage(currentPageID, true);
}

void DBBPlusTree::solveOverFlow(){
	int pagecnt;
	if (idp -> getFather() == -1)
		pagecnt = idf -> copyToNewPage(1);
	DBIndexDataPage* _idp = idf -> openIndexDataPage(1);
	DBIndexDataPage* __idp = idf -> openIndexDataPage(pagecnt);
	_idp -> refresh();
}