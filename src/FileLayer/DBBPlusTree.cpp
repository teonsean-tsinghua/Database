#include "DBBPlusTree.h"

DBBPlusTree::DBBPlusTree(const char* indexname, int _dataLen){
	idf = new DBIndexFile(indexname, _dataLen);
	dataLen = _dataLen;
}

void DBBPlusTree::insert(char* data, int len, int targetPage){
	int pageID = 1;
	idp = idf -> openIndexDataPage(1);
	if (idp -> getDataCnt() == 0){
		int a = 0;
		idp -> insert(data, len, &a, 0);
		return;
	}
	while (!idp -> getisLeaf()){
		pageID = idp -> searchIdx(data, len);
		idp = idf -> openIndexDataPage(pageID);
	}
	int status = idp -> insert(data, len, (unsigned int*)&targetPage, idp -> searchIdx(data, len));
	if (status == DBIndexDataPage::OVER_FLOW){
		solveOverFlow(data, len, pageID);
	}
}

void DBBPlusTree::deleteData(char* data, int len){
	int pageID = 1;
	while (!idp -> getisLeaf()){
		pageID = idp -> searchIdx(data, len);
		idp = idf -> openIndexDataPage(pageID);
	}
	int status = idp -> deleteByIdx(idp -> searchIdx(data, len));
	if (status == DBIndexDataPage::UNDER_FLOW){
		solveUnderFlow(data, len, pageID);
	}
}

int DBBPlusTree::search(char* data, int len){
	while(!idp -> getisLeaf()){
		idp = idf -> openIndexDataPage(idp -> searchIdx(data, len));
	}
	return idp -> searchIdx(data, len);
}

int DBBPlusTree::getBrother(char* data, int len, int pageID, bool &left){
	idp = idf -> openIndexDataPage(pageID);
	int idx;
	idx = idp -> searchIdx(data, len);
	if (idx == 0){
		idx = idx + 1;
		left = false;
	}
	else{
		idx = idx - 1;
		left = true;
	}
	return idp -> getPointerbyIdx(idx);
}

void DBBPlusTree::solveUnderFlow(char* data, int len, int pageID){
	idp -> forceDelete(idp -> searchIdx(data, len));
	int fatherPageID = idp -> getFather();
	bool isLeftBrother;
	if (fatherPageID != -1){
		int idx = getBrother(data, len, fatherPageID, isLeftBrother);
		if (idp -> getFloatSize() == 0){
			if (isLeftBrother == false)
				mergePage(pageID, idx, fatherPageID, data, len);
			else{
				mergePage(idx, pageID, fatherPageID, data, len);
			}
			idp = idf -> openIndexDataPage(fatherPageID);
			if (idf -> getFloatSize() < 0){
				solveUnderFlow(data, len, fatherPageID);
			}
		}
		else{
			if (isLeftBrother == false){
				rotateLeft(pageID, idx, fatherPageID, data, len);
			}
			else{
				rotateRight(idx, pageID, fatherPageID, data, len);
			}
		}
	}
	idf -> setAvailableOfIndexPage(currentPageID, true);
}

void DBBPlusTree::rotateLeft(int leftPageID, int rightPageID, int fatherPageID, char* data, int len){
	DBIndexDataPage* idp1 = idf -> openIndexDataPage(leftPageID);
	DBIndexDataPage* idp2 = idf -> openIndexDataPage(rightPageID);
	DBIndexDataPage* fatherDP = idf -> openIndexDataPage(fatherPageID);
	int idx = fatherDP -> updateIdx(fatherDP -> searchIdx(data, len), idp2 -> getDataHead());
	idp1 -> appendData(fatherDP -> getDataHead(), dataLen + sizeof(int));
	idp2 -> forceDelete(0);
}

void DBBPlusTree::rotateRight(int leftPageID, int rightPageID, int fatherPageID, char* data, int len){
	DBIndexDataPage* idp1 = idf -> openIndexDataPage(leftPageID);
	DBIndexDataPage* idp2 = idf -> openIndexDataPage(rightPageID);
	DBIndexDataPage* fatherDP = idf -> openIndexDataPage(fatherPageID);
	int idx = fatherDP -> updateIdx(fatherDP -> searchIdx(data, len), idp1 -> getOffsetbyIdx(idp1 -> getDataCnt() - 1));
	idp2 -> insert(fatherDP -> getDataHead(), dataLen, idp1 -> getPointerbyIdx(0), 0);
	idp1 -> forceDelete(idp2 -> getDataCnt() - 1);
}

void DBBPlusTree::mergePage(int leftPageID, int rightPageID, int fatherPageID, char* data, int len){
	DBIndexDataPage* idp1 = idf -> openIndexDataPage(leftPageID);
	DBIndexDataPage* idp2 = idf -> openIndexDataPage(rightPageID);
	DBIndexDataPage* fatherDP = idf -> openIndexDataPage(fatherPageID);
	int idx = fatherDP -> searchIdx(data, len);
	idp1 -> appendData(fatherDP -> getOffsetbyIdx(idx), dataLen);
	idp1 -> appendData(idp2 -> getDataHead(), idp2 -> getDataSize());
}

void DBBPlusTree::solveOverFlow(char* data, int len, int pageID){
	int pagecnt;
	int fatherPageID = idp -> getFather();
	pagecnt = idp -> copyToNewPage(pageID);
	DBIndexDataPage fatherDP = idf -> openIndexDataPage(fatherPageID);
	if(idf -> getDataCnt() == idf -> getMaxSize()){}
	else{
		fatherPageID -> insert(fatherDP -> getDataHead(), dataLen, pagecnt, fatherDP -> searchIdx(data, len));
	}
}