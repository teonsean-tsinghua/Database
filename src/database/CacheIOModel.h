#ifndef CACHEIOMODEL_H_INCLUDED
#define CACHEIOMODEL_H_INCLUDED

#include"Include.h"

void readInt(char* cache, int *n);

void writeInt(char* cache, int n);

void readString(char* cache, std::string& str, int len);

void writeString(char* cache, std::string& str, int len);

void readCharToBool(char* cache, bool* b);

void writeBoolToChar(char* cache, bool b);

void readData(char* cache, char* data, int len);

void writeData(char* cache, char* data, int len);

void copyData(char* src, char* dest, int len);

void read_id(char* cache, char* data);

void write_id(char* cache);


#endif // CACHEIOMODEL_H_INCLUDED
