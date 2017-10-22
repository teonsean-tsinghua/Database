#include "DBRecordLinker.h"
#include "../utils/DBLog.h"
#include "../utils/pagedef.h"
#include <ctime>

DBLinkedRecord::DBLinkedRecord(DBRecordInfo _record, bool _isDebugMode):record(_record){
	debugmode = _isDebugMode;
	if (debugmode){
		char* msg = new char[64];
    	sprintf(msg, "Set a new record.");
    	log(std::string(msg));
    }
    keyvalue = rand() * rand() * rand();
}

void DBLinkedRecord::setPreviousRecord(DBLinkedRecord *_previousRecord){
	previousRecord = _previousRecord;
	if (debugmode){
		char* msg = new char[64];
		sprintf(msg, "Set a previousRecord.");
		log(std::string(msg));
	}
}

void DBLinkedRecord::setNextRecord(DBLinkedRecord *_nextRecord){
	nextRecord = _nextRecord;
	if (debugmode){
		char* msg = new char[64];
		sprintf(msg, "set a nextRecord.");
		log(std::string(msg));
	}
}

DBLinkedRecord* DBLinkedRecord::getPreviousRecord(){
	if (debugmode){
		char* msg = new char[64];
		sprintf(msg, "Get a previous record.");
		log(std::string(msg));
	}
	return previousRecord;
}

DBLinkedRecord* DBLinkedRecord::getNextRecord(){
	if (debugmode){
		char* msg = new char[64];
		sprintf(msg, "Get a next record.");
		log(std::string(msg));
	}
	return nextRecord;
}

int DBLinkedRecord::getKeyValue(){
	return keyvalue;
}

DBRecordLinker::DBRecordLinker(bool _debugmode){
	debugmode = _debugmode;
	linker = NULL;
}

bool DBRecordLinker::insertRecord(DBLinkedRecord _linkedRecord, int _keyvalue){
	if (linker == NULL){
		linker = &_linkedRecord;
		linkersize++;
		return true;
	}
	else{
		DBLinkedRecord * recorditer;
		while(recorditer -> getPreviousRecord() != NULL)
			recorditer = recorditer -> getPreviousRecord();
		while(recorditer -> getNextRecord() != NULL){
			if (recorditer -> getKeyValue() == _keyvalue){
				linkersize++;
				if (linkersize * sizeof(DBLinkedRecord) > PAGE_SIZE){
					throwLastrecord();
					linkersize--;
				}
				DBLinkedRecord record = _linkedRecord;
				record.setPreviousRecord(recorditer);
				record.setNextRecord(recorditer -> getNextRecord());
				recorditer -> getPreviousRecord() -> setNextRecord(&record);
				recorditer -> getNextRecord() -> setPreviousRecord(&record);
				return true;
			}
			recorditer = recorditer -> getNextRecord();
		}
	}
	return false;
}

bool DBRecordLinker::deleteRecord(int _keyvalue){
	if (linker == NULL){
		return false;
	}
	else{
		DBLinkedRecord* recorditer = linker;
		while(recorditer -> getPreviousRecord() != NULL)
			recorditer = recorditer -> getPreviousRecord();
		while(recorditer -> getNextRecord() != NULL){
			if (recorditer -> getKeyValue() == _keyvalue){
				linkersize--;
				recorditer -> getPreviousRecord() -> setNextRecord(recorditer -> getNextRecord());
				recorditer -> getNextRecord() -> setPreviousRecord(recorditer -> getPreviousRecord());
				delete recorditer;
				return true;
			}
			recorditer = recorditer -> getNextRecord();
		}
	}
	return false;
}

DBLinkedRecord* DBRecordLinker::searchRecord(int _keyvalue){
	if (this -> linker == NULL){
		return NULL;
	}
	else{
		DBLinkedRecord* recorditer = this -> linker;
		while(recorditer -> getPreviousRecord() != NULL)
			recorditer = recorditer -> getPreviousRecord();
		if (recorditer -> getKeyValue() == _keyvalue)
			return recorditer;
		while(recorditer -> getNextRecord() != NULL){
			if (recorditer -> getKeyValue() == _keyvalue){
				return recorditer;
			}
			recorditer = recorditer -> getNextRecord();
		}
	}
	return NULL;
}

DBRecordLinker* DBRecordLinker::getPreviousLinker(){
	return this -> _previouslinker;
}

DBRecordLinker* DBRecordLinker::getNextRecord(){
	return this -> _nextlinker;
}

void DBRecordLinker::throwLastRecord(){
	if (this == NULL){
		if (debugmode){
			char msg[64];
			sprintf("Unreasonable throw for last record.");
			log(std::string(msg));
		}
	}
	DBRecordLinker recorditer = this -> linker;
	while(recorditer.getNextRecord() != NULL)
		recorditer = recorditer.getNextRecord();
	DBRecordLinker* thepage;
	if (this -> getNextLinker() == NULL){
		thepage = new DBRecordLinker(_debugmode);
	}
	else{
		thepage = this -> getNextLinker(); 
	}
	thepage -> acceptRecord(recorditer);
}

void DBRecordLinker::acceptFirstRecord(DBLinkedRecord dlr){
	this -> linkersize++;
	if ((this -> linkersize) * sizeof(DBLinkedRecord) > PAGE_SIZE){
		this -> throwLastRecord();
		this -> linkersize--;
	}
	DBRecordLinker* recorditer = this -> linker;
	while(recorditer -> getPreviousRecord() != NULL)
		recorditer = recorditer -> getPreviousRecord();
	recorditer -> setPreviousRecord(&dlr);
	dlr.setNextRecord(recorditer);
}