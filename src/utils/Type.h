#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#include<cstring>
#include<vector>

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
    const static int FIRST_TYPE = _ID;
    const static int LAST_TYPE = VARCHAR;

    const static int DATA_FILE_DESC_PAGE = 0;
    const static int DATA_PAGE = DATA_FILE_DESC_PAGE + 1;
    const static int USAGE_PAGE = DATA_PAGE + 1;
    const static int INDEX_FILE_DESC_PAGE = USAGE_PAGE + 1;
    const static int INDEX_INTERNAL_PAGE = INDEX_FILE_DESC_PAGE + 1;
    const static int INDEX_LEAF_PAGE = INDEX_INTERNAL_PAGE + 1;
    const static int BUCKET_PAGE = INDEX_LEAF_PAGE + 1;
    const static int FIRST_PAGE_TYPE = DATA_FILE_DESC_PAGE;
    const static int LAST_PAGE_TYPE = BUCKET_PAGE;

    static int typeSize(int type);

    static const char* typeName(int type);

    static const char* pageName(int type);
};

class DataType
{

};

class RecordInfo;

class PrimKey: public DataType
{

public:
	static RecordInfo* ri;

	DataType keys[];

	bool friend operator < (PrimKey& one, PrimKey& other);

	bool friend operator > (PrimKey& one, PrimKey& other);

	bool friend operator <= (PrimKey& one, PrimKey& other);

	bool friend operator >= (PrimKey& one, PrimKey& other);

	bool friend operator == (PrimKey& one, PrimKey& other);

	bool friend operator != (PrimKey& one, PrimKey& other);

};

class VarcharType: public DataType
{

public:
	char v[];

	bool friend operator < (VarcharType& one, VarcharType& other);

	bool friend operator > (VarcharType& one, VarcharType& other);

	bool friend operator <= (VarcharType& one, VarcharType& other);

	bool friend operator >= (VarcharType& one, VarcharType& other);

	bool friend operator == (VarcharType& one, VarcharType& other);

	bool friend operator != (VarcharType& one, VarcharType& other);

};

class DateType: public DataType
{

public:
	char v[8];

	bool friend operator < (DateType& one, DateType& other);

	bool friend operator > (DateType& one, DateType& other);

	bool friend operator <= (DateType& one, DateType& other);

	bool friend operator >= (DateType& one, DateType& other);

	bool friend operator == (DateType& one, DateType& other);

	bool friend operator != (DateType& one, DateType& other);

};

class FloatType: public DataType
{

public:
	float v;

	bool friend operator < (FloatType& one, FloatType& other);

	bool friend operator > (FloatType& one, FloatType& other);

	bool friend operator <= (FloatType& one, FloatType& other);

	bool friend operator >= (FloatType& one, FloatType& other);

	bool friend operator == (FloatType& one, FloatType& other);

	bool friend operator != (FloatType& one, FloatType& other);

};

class IntType: public DataType
{

public:
	int v;

	bool friend operator < (IntType& one, IntType& other);

	bool friend operator > (IntType& one, IntType& other);

	bool friend operator <= (IntType& one, IntType& other);

	bool friend operator >= (IntType& one, IntType& other);

	bool friend operator == (IntType& one, IntType& other);

	bool friend operator != (IntType& one, IntType& other);

};

class IDType: public DataType
{

public:
	unsigned long long ts;
	unsigned int ca;
	unsigned short pid;
	unsigned short rd;

	bool friend operator < (IDType& one, IDType& other);

	bool friend operator > (IDType& one, IDType& other);

	bool friend operator <= (IDType& one, IDType& other);

	bool friend operator >= (IDType& one, IDType& other);

	bool friend operator == (IDType& one, IDType& other);

	bool friend operator != (IDType& one, IDType& other);
};

#endif // TYPE_H_INCLUDED
