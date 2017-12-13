#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include"Info.h"
#include"Type.h"
#include<iostream>
#include<iomanip>

class DataFile;

class Table
{
public:
	static void print(std::vector<bool>& selected, RecordInfo* ri, SelectResult& sr, DataFile* df);

};

#endif // TABLE_H_INCLUDED