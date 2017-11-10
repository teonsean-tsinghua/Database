#ifndef DBCACHEIOMODEL_H_INCLUDED
#define DBCACHEIOMODEL_H_INCLUDED

#include"../DBInclude.h"
#include"DBFileIOModel.h"

void readInt(BufType cache, int *n);

void writeInt(BufType cache, int n);

void readString(char* cache, std::string& str, int len);

void writeString(char* cache, std::string& str, int len);

void readCharToBool(BufType cache, bool* b);

void writeBoolToChar(BufType cache, bool b);

#endif // DBCACHEIOMODEL_H_INCLUDED
