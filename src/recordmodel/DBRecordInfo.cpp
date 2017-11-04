#include "DBRecordInfo.h"
#include "../utils/DBLog.h"
#include <cstdio>
#include "../utils/pagedef.h"
#include <cstring>

const int DBRecordInfo::SIZEOF_TYPE[] = {8 * sizeof(char),
                                        sizeof(int),
                                        sizeof(long),
                                        sizeof(char),
                                        sizeof(char),
                                        sizeof(float),
                                        sizeof(double),
                                        256 * sizeof(char),
                                        1024 * sizeof(char)};

const std::string DBRecordInfo::NAMEOF_TYPE[] = {"RID",
                                                "int",
                                                "long",
                                                "boolean",
                                                "char",
                                                "float"
                                                "double",
                                                "string",
                                                "long string"};

DBRecordInfo::DBRecordInfo(std::vector<std::string> &_names,
                           std::vector<int> &_types,
                           std::vector<int> &_lengths)
{
    column_count = _names.size();
    names = new std::string[column_count];
    types = new int[column_count];
    offsets = new int[column_count];
    lengths = new int[column_count];
    offsets[0] = 0;
    for(int i = 0; i < _names.size(); i++)
    {
        names[i] = _names[i];
        types[i] = _types[i];
        lengths[i] = _lengths[i];
        if(i > 0){
            offsets[i] = offsets[i - 1] + SIZEOF_TYPE[types[i - 1]] * lengths[i - 1] + 2;
        }
        char* msg = new char[64];
        sprintf(msg, "Column %s is of type %s, at offset %d", names[i].c_str(), NAMEOF_TYPE[types[i]].c_str(), offsets[i]);
        log(std::string(msg));
    }
    totalLength = offsets[_names.size() - 1] + SIZEOF_TYPE[types[_names.size() - 1]] * lengths[_names.size() - 1] + 2;
    char* msg = new char[64];
    sprintf(msg, "Record length is %d.", totalLength);
    log(std::string(msg));
}

unsigned char* DBRecordInfo::toBinary(){
    // When transferring succeed, can just write the data from return loc,
    // but when transferring failed, will return a NULL pointer.
    // will be transfered as follows:
    // [2 bytes total length], [2 bytes number of data]
    // following [number of data] lines:
    // [1 bytes data type], [2 bytes data length], [2 bytes offset in data page], [2 bytes length of name of data], [data]
    unsigned char* infoPageData = new unsigned char[PAGE_SIZE << 1];
    if (totalLength > 8190){
        return NULL;
    }
    char* msg = new char[64];
    infoPageData[0] = (unsigned char)(totalLength >> 8);
    infoPageData[1] = (unsigned char)totalLength % 256;
    infoPageData[2] = (unsigned char)(column_count >> 8);
    infoPageData[3] = (unsigned char)column_count % 256;
    int bytescnt = 4;
    for(int i = 0; i < column_count; i++){
        if (bytescnt > 8192)
            return NULL;
        infoPageData[bytescnt] = (unsigned char)types[i];
        bytescnt++;
        infoPageData[bytescnt] = (unsigned char)(lengths[i] >> 8);
        bytescnt++;
        infoPageData[bytescnt] = (unsigned char)lengths[i] % 256;
        bytescnt++;
        infoPageData[bytescnt] = (unsigned char)(offsets[i] >> 8);
        bytescnt++;
        infoPageData[bytescnt] = (unsigned char)offsets[i] % 256;
        bytescnt++;
        infoPageData[bytescnt] = (unsigned char)(names[i].length() >> 8);
        bytescnt++;
        infoPageData[bytescnt] = (unsigned char)names[i].length() % 256;
        bytescnt++;
        for(int j = 0; j < names[i].length(); j++){
            infoPageData[bytescnt] = (unsigned char)names[i][j];
            bytescnt++;
        }
    }
    return infoPageData;
}

int DBRecordInfo::fromBinary(unsigned char* binArray){
    if (names != NULL){
        delete[] names;
    }
    if (types != NULL){
        delete[] types;
    }
    if (lengths != NULL){
        delete[] lengths;
    }
    if (offsets != NULL){
        delete[] offsets;
    }
    totalLength = (int)binArray[0] * 256 + (int) binArray[1];
    column_count = (int)binArray[2] * 256 + (int) binArray[3];
    int bytescnt = 4;
    names = new std::string[column_count];
    types = new int[column_count];
    lengths = new int[column_count];
    offsets = new int[column_count];
    for(int i = 0; i < column_count; i++){
        types[i] = int(binArray[bytescnt]);
        bytescnt++;
        lengths[i] = int(binArray[bytescnt]) * 256 + int(binArray[bytescnt + 1]);
        bytescnt += 2;
        offsets[i] = int(binArray[bytescnt]) * 256 + int(binArray[bytescnt + 1]);
        bytescnt += 2;
        int _nameLen = int(binArray[bytescnt]) * 256 + int(binArray[bytescnt + 1]);
        bytescnt += 2;
        char* _name = new char[512];
        for(int j = 0; j < _nameLen; j++){
            _name[j] = binArray[bytescnt];
            bytescnt++;
        }
        names[i] = _name;
        char* msg = new char[64];
        sprintf(msg, "Loaded data type %s", _name);
        log(std::string(msg));
    }
    return SUCCEED;
}

int DBRecordInfo::getOffsetByName(const char* name){
    for(int i = 0; i < column_count; i++){
        if (strcmp(names[i].c_str(), name) == 0){
            char* msg = new char[64];
            sprintf(msg, "Index found at offset %d.", offsets[i]);
            log(std::string(msg));
            return offsets[i];
        }
    }
    return -1;
}

DBRecordInfo::~DBRecordInfo(){
    delete[] names;
    delete[] types;
    delete[] lengths;
    delete[] offsets;
}
