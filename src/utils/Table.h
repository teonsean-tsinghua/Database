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

    static std::vector<std::vector<int> > idxes;
    static std::vector<std::vector<int> > widths;

public:

	static void printHeader(std::vector<bool>& selected, RecordInfo* ri);

	static void printHeader(std::vector<std::vector<bool> >& selected, std::vector<RecordInfo*>& ri, std::vector<std::string>& tbnames);

	static void printRow(std::vector<void*>& row, RecordInfo* ri);

	static void printRow(std::vector<std::vector<void*> >& row, std::vector<RecordInfo*>& ri);

};

#endif // TABLE_H_INCLUDED
