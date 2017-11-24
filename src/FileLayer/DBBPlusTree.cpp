//#include "DBBPlusTree.h"
//
//DBBPlusTree::DBBPlusTree(const char* indexname, int _dataLen){
//	idf = new DBIndexFile(indexname, _dataLen);
//	dataLen = _dataLen;
//}
//
//void DBBPlusTree::insert(char* data, int len, int targetPage, int offset){
//	int pageID = 1;
//	idp = idf -> openIndexDataPage(1);
//	if (idp -> getDataCnt() == 0){
//		idp -> insert(data, len, targetPage, 0, offset);
//		return;
//	}
//	while (!idp -> getIsLeaf()){
//		pageID = idp -> search(data, len);
//		idp = idf -> openIndexDataPage(pageID);
//	}
//	int status = idp -> insert(data, len, targetPage, idp -> searchIdx(data, len), offset);
//	if (status == DBIndexDataPage::OVER_FLOW){
//		solveOverFlow(data, len, pageID);
//	}
//}
//
//void DBBPlusTree::deleteData(char* data, int len){
//	int pageID = 1;
//	while (!idp -> getIsLeaf()){
//		pageID = idp -> search(data, len);
//		idp = idf -> openIndexDataPage(pageID);
//	}
//	int status = idp -> deleteByIdx(idp -> search(data, len));
//	if (status == DBIndexDataPage::UNDER_FLOW){
//		solveUnderFlow(data, len, pageID);
//	}
//}
//
//int DBBPlusTree::search(char* data, int len){
//	while(!idp -> getIsLeaf()){
//		idp = idf -> openIndexDataPage(idp -> search(data, len));
//	}
//	return idp -> search(data, len);
//}
//
//int DBBPlusTree::getBrother(char* data, int len, int pageID, bool &left){
//	idp = idf -> openIndexDataPage(pageID);
//	int idx;
//	idx = idp -> search(data, len);
//	if (idx == 0){
//		idx = idx + 1;
//		left = false;
//	}
//	else{
//		idx = idx - 1;
//		left = true;
//	}
//	return idp -> getPointerByIdx(idx);
//}
//
//void DBBPlusTree::solveUnderFlow(char* data, int len, int pageID){
//	idp -> forceDelete(idp -> search(data, len));
//	int fatherPageID = idp -> getFather();
//	bool isLeftBrother;
//	if (fatherPageID != -1){
//		int idx = getBrother(data, len, fatherPageID, isLeftBrother);
//		if (idp -> getFloatSize() == 0){
//			if (isLeftBrother == false)
//				mergePage(pageID, idx, fatherPageID, data, len);
//			else{
//				mergePage(idx, pageID, fatherPageID, data, len);
//			}
//			idp = idf -> openIndexDataPage(fatherPageID);
//			if (idp -> getFloatSize() < 0){
//				solveUnderFlow(data, len, fatherPageID);
//			}
//		}
//		else{
//			if (isLeftBrother == false){
//				rotateLeft(pageID, idx, fatherPageID, data, len);
//			}
//			else{
//				rotateRight(idx, pageID, fatherPageID, data, len);
//			}
//		}
//	}
//	idf -> setAvailableOfIndexPage(currentPageID, true);
//}
//
//void DBBPlusTree::rotateLeft(int leftPageID, int rightPageID, int fatherPageID, char* data, int len){
//	DBIndexDataPage* idp1 = idf -> openIndexDataPage(leftPageID);
//	DBIndexDataPage* idp2 = idf -> openIndexDataPage(rightPageID);
//	DBIndexDataPage* fatherDP = idf -> openIndexDataPage(fatherPageID);
//	fatherDP -> updateIdx((char*)idp2 -> getDataHead(), fatherDP -> search(data, len));
//	idp1 -> appendData((char*)fatherDP -> getDataHead(), dataLen + sizeof(int));
//	idp2 -> forceDelete(0);
//}
//
//void DBBPlusTree::rotateRight(int leftPageID, int rightPageID, int fatherPageID, char* data, int len){
//	DBIndexDataPage* idp1 = idf -> openIndexDataPage(leftPageID);
//	DBIndexDataPage* idp2 = idf -> openIndexDataPage(rightPageID);
//	DBIndexDataPage* fatherDP = idf -> openIndexDataPage(fatherPageID);
//	fatherDP -> updateIdx((char*)idp1 -> getDataByIdx(idp1 -> getDataCnt() - 1), fatherDP -> search(data, len));
//	idp2 -> insert((char*)fatherDP -> getDataHead(), dataLen, idp1 -> getPointerByIdx(0), 0, idp1 -> getDataOffsetByIdx(0));
//	idp1 -> forceDelete(idp2 -> getDataCnt() - 1);
//}
//
//void DBBPlusTree::mergePage(int leftPageID, int rightPageID, int fatherPageID, char* data, int len){
//	DBIndexDataPage* idp1 = idf -> openIndexDataPage(leftPageID);
//	DBIndexDataPage* idp2 = idf -> openIndexDataPage(rightPageID);
//	DBIndexDataPage* fatherDP = idf -> openIndexDataPage(fatherPageID);
//	int idx = fatherDP -> search(data, len);
//	idp1 -> appendData((char*)fatherDP -> getDataByIdx(idx), dataLen);
//	idp1 -> appendData((char*)idp2 -> getDataHead(), idp2 -> getDataSize());
//}
//
//void DBBPlusTree::solveOverFlow(char* data, int len, int pageID){
//	int pagecnt;
//	int fatherPageID = idp -> getFather();
//	pagecnt = idf -> copyToNewPage(pageID);
//	DBIndexDataPage* fatherDP = idf -> openIndexDataPage(fatherPageID);
//	if(fatherDP -> getDataCnt() == fatherDP -> getMaxSize()){}
//	else{
//		fatherDP -> insert((char*)fatherDP -> getDataHead(), dataLen, pagecnt, fatherDP -> search(data, len), 0);
//	}
//}
//
//void DBBPlusTree::closeFile(){
//	idf -> closeFile();
//}
//
//void DBBPlusTree::createFile(){
//	idf -> createFile();
//}
//
//void DBBPlusTree::print(){
//	for(int i = 1; i < 2 ;i++){
//		DBIndexDataPage* idp = idf -> openIndexDataPage(i);
//		idp -> print();
//	}
//}
