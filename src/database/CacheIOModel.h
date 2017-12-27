#ifndef CACHEIOMODEL_H_INCLUDED
#define CACHEIOMODEL_H_INCLUDED

#include<cstring>
#include<string>

int readInt(char* cache);

void writeInt(char* cache, int n);

std::string readString(char* cache, int len);

void writeString(char* cache, std::string& str, int len);

bool readBool(char* cache);

void writeBool(char* cache, bool b);

void copyData(char* src, char* dest, int len);

#endif // CACHEIOMODEL_H_INCLUDED
