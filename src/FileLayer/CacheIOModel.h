#ifndef CACHEIOMODEL_H_INCLUDED
#define CACHEIOMODEL_H_INCLUDED

#include"../Include.h"
#include"FileIOModel.h"

void readInt(BufType cache, int *n);

void readUnsignedInt(BufType cache, unsigned int* n);

void writeInt(BufType cache, int n);

void writeUnsignedInt(BufType cache, unsigned int n);

void readString(char* cache, std::string& str, int len);

void writeString(char* cache, std::string& str, int len);

void readCharToBool(BufType cache, bool* b);

void writeBoolToChar(BufType cache, bool b);

void readData(BufType cache, char* data, int len);

void writeData(BufType cache, char* data, int len);

void copyData(BufType src, BufType dest, int len);

void read_id(BufType cache, char* data);

void write_id(BufType cache);


#endif // CACHEIOMODEL_H_INCLUDED
