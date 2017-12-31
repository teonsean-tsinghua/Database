#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include"Info.h"
#include"Type.h"
#include<iostream>
#include<iomanip>

class DataFile;

class Table
{
    static std::vector<int> idx;
    static std::vector<int> width;

public:
	static void print(std::vector<bool>& selected, RecordInfo* ri, SelectResult& sr, DataFile* df);

	static void printHeader(std::vector<bool>& selected, RecordInfo* ri);

	static void printRow(std::vector<void*>& row, RecordInfo* ri);

};

#endif // TABLE_H_INCLUDED
