#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#include<cstring>
#include<vector>
#include<iostream>
#include<string>

class Type
{
    const static int typeSize_[];
    const static char* const typeName_[];
    const static char* const pageName_[];

public:
    const static int _ID = 0;   // _id: time stamp for first 8 bytes, cache address for next 4 bytes, pid for next 2 bytes, a rand number for last 2 bytes;
    const static int INT = _ID + 1;
    const static int FLOAT = INT + 1;
    const static int DATE = FLOAT + 1;
    const static int VARCHAR = DATE + 1;
    const static int PRIMARYKEY = VARCHAR + 1;
    const static int FIRST_TYPE = _ID;
    const static int LAST_TYPE = VARCHAR;

    const static int DATA_FILE_DESC_PAGE = 0;
    const static int DATA_PAGE = DATA_FILE_DESC_PAGE + 1;
    const static int USAGE_PAGE = DATA_PAGE + 1;
    const static int INDEX_FILE_DESC_PAGE = USAGE_PAGE + 1;
    const static int INTERNAL_PAGE = INDEX_FILE_DESC_PAGE + 1;
    const static int LEAF_PAGE = INTERNAL_PAGE + 1;
    const static int BUCKET_PAGE = LEAF_PAGE + 1;
    const static int FIRST_PAGE_TYPE = DATA_FILE_DESC_PAGE;
    const static int LAST_PAGE_TYPE = BUCKET_PAGE;

    static int typeSize(int type);

    static const char* typeName(int type);

    static const char* pageName(int type);
};

class DataType
{

};

class Record
{
public:
    char* buf;
};

class RecordInfo;

class PrimKey: public DataType
{

public:
	static RecordInfo* ri;

	DataType keys[8192];

	void friend operator << (PrimKey& one, PrimKey& other);

	bool friend operator < (PrimKey& one, PrimKey& other);

	bool friend operator > (PrimKey& one, PrimKey& other);

	bool friend operator <= (PrimKey& one, PrimKey& other);

	bool friend operator >= (PrimKey& one, PrimKey& other);

	bool friend operator == (PrimKey& one, PrimKey& other);

	bool friend operator != (PrimKey& one, PrimKey& other);

	friend std::ostream& operator << (std::ostream& out, PrimKey& one);

};

class VarcharType: public DataType
{

public:
	char v[8192];

	void friend operator << (VarcharType& one, VarcharType& other);

	bool friend operator < (VarcharType& one, VarcharType& other);

	bool friend operator > (VarcharType& one, VarcharType& other);

	bool friend operator <= (VarcharType& one, VarcharType& other);

	bool friend operator >= (VarcharType& one, VarcharType& other);

	bool friend operator == (VarcharType& one, VarcharType& other);

	bool friend operator != (VarcharType& one, VarcharType& other);

	friend std::ostream& operator << (std::ostream& out, VarcharType& one);

};

class DateType: public DataType
{

public:
	char v[8];

	void friend operator << (DateType& one, DateType& other);

	bool friend operator < (DateType& one, DateType& other);

	bool friend operator > (DateType& one, DateType& other);

	bool friend operator <= (DateType& one, DateType& other);

	bool friend operator >= (DateType& one, DateType& other);

	bool friend operator == (DateType& one, DateType& other);

	bool friend operator != (DateType& one, DateType& other);

	friend std::ostream& operator << (std::ostream& out, DateType& one);

};

class FloatType: public DataType
{

public:
	float v;

	void friend operator << (FloatType& one, FloatType& other);

	bool friend operator < (FloatType& one, FloatType& other);

	bool friend operator > (FloatType& one, FloatType& other);

	bool friend operator <= (FloatType& one, FloatType& other);

	bool friend operator >= (FloatType& one, FloatType& other);

	bool friend operator == (FloatType& one, FloatType& other);

	bool friend operator != (FloatType& one, FloatType& other);

	friend std::ostream& operator << (std::ostream& out, FloatType& one);

};

class IntType: public DataType
{

public:
	int v;

	void friend operator << (IntType& one, IntType& other);

	bool friend operator < (IntType& one, IntType& other);

	bool friend operator > (IntType& one, IntType& other);

	bool friend operator <= (IntType& one, IntType& other);

	bool friend operator >= (IntType& one, IntType& other);

	bool friend operator == (IntType& one, IntType& other);

	bool friend operator != (IntType& one, IntType& other);

	friend std::ostream& operator << (std::ostream& out, IntType& one);

};

class IDType: public DataType
{

public:
	unsigned long long ts;
	unsigned int ca;
	unsigned short pid;
	unsigned short rd;

	void friend operator << (IDType& one, IDType& other);

	bool friend operator < (IDType& one, IDType& other);

	bool friend operator > (IDType& one, IDType& other);

	bool friend operator <= (IDType& one, IDType& other);

	bool friend operator >= (IDType& one, IDType& other);

	bool friend operator == (IDType& one, IDType& other);

	bool friend operator != (IDType& one, IDType& other);

	friend std::ostream& operator << (std::ostream& out, IDType& one);
};

#endif // TYPE_H_INCLUDED
