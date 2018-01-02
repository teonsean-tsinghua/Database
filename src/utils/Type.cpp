#include"Type.h"
#include<cstdio>
#include"CacheIOModel.h"
#include"Info.h"

RecordInfo* PrimKey::ri;

const int Type::typeSize_[] = {16, sizeof(int), sizeof(float), 10, 0};

const char* const Type::typeName_[] = {"_ID", "INT", "FLOAT", "DATE", "VARCHAR"};

const char* const Type::pageName_[] = {"Data file Desc page",
									   "Data page",
									   "Usage page",
							  		   "Index file description page",
									   "Internal node page",
									   "Leaf node page",
									   "Bucket page"};

int Type::typeSize(int type)
{
    if(type < FIRST_TYPE || type > LAST_TYPE)
    {
        return -1;
    }
    return typeSize_[type];
}

const char* Type::typeName(int type)
{
    if(type < FIRST_TYPE || type > LAST_TYPE)
    {
        return NULL;
    }
    return typeName_[type];
}

const char* Type::pageName(int type)
{
    if(type < FIRST_PAGE_TYPE || type > LAST_PAGE_TYPE)
    {
        return NULL;
    }
    return pageName_[type];
}

bool operator == (IDType const& one, IDType const& other)
{
	return one.ts == other.ts && one.ca == other.ca && one.pid == other.pid && one.rd == other.rd;
}

void operator << (IDType& one, IDType const& other)
{
	one.ts = other.ts;
	one.ca = other.ca;
	one.pid = other.pid;
	one.rd = other.rd;
}

bool operator != (IDType const& one, IDType const& other)
{
	return !(one == other);
}

bool operator < (IDType const& one, IDType const& other)
{
	return one.ts < other.ts ||
			(one.ts == other.ts && one.ca < other.ca) ||
			(one.ts == other.ts && one.ca == other.ca && one.pid < other.pid) ||
			(one.ts == other.ts && one.ca == other.ca && one.pid == other.pid && one.rd < other.rd);
}

bool operator > (IDType const& one, IDType const& other)
{
	return one.ts > other.ts ||
			(one.ts == other.ts && one.ca > other.ca) ||
			(one.ts == other.ts && one.ca == other.ca && one.pid > other.pid) ||
			(one.ts == other.ts && one.ca == other.ca && one.pid == other.pid && one.rd > other.rd);
}

bool operator <= (IDType const& one, IDType const& other)
{
	return !(one > other);
}

bool operator >= (IDType const& one, IDType const& other)
{
	return !(one < other);
}

std::ostream& operator << (std::ostream& out, IDType const& one)
{
    out << read_id((char*)&one);
    return out;
}

bool operator == (IntType const& one, IntType const& other)
{
	return one.v == other.v;
}

bool operator != (IntType const& one, IntType const& other)
{
	return one.v != other.v;
}

bool operator < (IntType const& one, IntType const& other)
{
	return one.v < other.v;
}

bool operator > (IntType const& one, IntType const& other)
{
	return one.v > other.v;
}

void operator << (IntType& one, IntType const& other)
{
	one.v = other.v;
}

bool operator <= (IntType const& one, IntType const& other)
{
	return one.v <= other.v;
}

bool operator >= (IntType const& one, IntType const& other)
{
	return one.v >= other.v;
}

std::ostream& operator << (std::ostream& out, IntType const& one)
{
    out << one.v;;
    return out;
}

bool operator == (FloatType const& one, FloatType const& other)
{
	return one.v == other.v;
}

bool operator != (FloatType const& one, FloatType const& other)
{
	return one.v != other.v;
}

bool operator < (FloatType const& one, FloatType const& other)
{
	return one.v < other.v;
}

bool operator > (FloatType const& one, FloatType const& other)
{
	return one.v > other.v;
}

void operator << (FloatType& one, FloatType const& other)
{
	one.v = other.v;
}

bool operator <= (FloatType const& one, FloatType const& other)
{
	return one.v <= other.v;
}

bool operator >= (FloatType const& one, FloatType const& other)
{
	return one.v >= other.v;
}

std::ostream& operator << (std::ostream& out, FloatType const& one)
{
    out << one.v;;
    return out;
}

bool operator == (DateType const& one, DateType const& other)
{
	for(int i = 0; i < 8; i++)
	{
		if(one.v[i] != other.v[i])
		{
			return false;
		}
	}
	return true;
}

bool operator != (DateType const& one, DateType const& other)
{
	return !(one == other);
}

void operator << (DateType& one, DateType const& other)
{
	strncpy(one.v, other.v, 8);
}

bool operator < (DateType const& one, DateType const& other)
{
	for(int i = 0; i < 8; i++)
	{
		if(one.v[i] < other.v[i])
		{
			return true;
		}
		else if(one.v[i] > other.v[i])
		{
			return false;
		}
	}
	return false;
}

bool operator > (DateType const& one, DateType const& other)
{
	for(int i = 0; i < 8; i++)
	{
		if(one.v[i] < other.v[i])
		{
			return false;
		}
		else if(one.v[i] > other.v[i])
		{
			return true;
		}
	}
	return false;
}

bool operator <= (DateType const& one, DateType const& other)
{

	for(int i = 0; i < 8; i++)
	{
		if(one.v[i] < other.v[i])
		{
			return true;
		}
		else if(one.v[i] > other.v[i])
		{
			return false;
		}
	}
	return true;
}

bool operator >= (DateType const& one, DateType const& other)
{
	for(int i = 0; i < 8; i++)
	{
		if(one.v[i] < other.v[i])
		{
			return false;
		}
		else if(one.v[i] > other.v[i])
		{
			return true;
		}
	}
	return true;
}

std::ostream& operator << (std::ostream& out, DateType const& one)
{
    std::string str;
    str = str.assign(one.v, 10);
    out << str;;
    return out;
}

bool operator < (PrimKey const& one, PrimKey const& other)
{
	const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	const char* ptr = (const char*)&one;
	const char* ptr2 = (const char*)&other;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
		switch(type)
		{
		case Type::_ID:
			if(*(const IDType*)(ptr) < *(const IDType*)(ptr2))
			{
				return true;
			}
			if(*(const IDType*)(ptr) > *(const IDType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::INT:
			if(*(const IntType*)(ptr) < *(const IntType*)(ptr2))
			{
				return true;
			}
			if(*(const IntType*)(ptr) > *(const IntType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::FLOAT:
			if(*(const FloatType*)(ptr) < *(const FloatType*)(ptr2))
			{
				return true;
			}
			if(*(const FloatType*)(ptr) > *(const FloatType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::DATE:
			if(*(const DateType*)(ptr) < *(const DateType*)(ptr2))
			{
				return true;
			}
			if(*(const DateType*)(ptr) > *(const DateType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::VARCHAR:
			if(*(const VarcharType*)(ptr) < *(const VarcharType*)(ptr2))
			{
				return true;
			}
			if(*(const VarcharType*)(ptr) > *(const VarcharType*)(ptr2))
			{
				return false;
			}
			break;
		}
		ptr += len;
		ptr2 += len;
	}
	return false;
}

bool operator > (PrimKey const& one, PrimKey const& other)
{
	const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	const char* ptr = (const char*)&one;
	const char* ptr2 = (const char*)&other;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
		switch(type)
		{
		case Type::_ID:
			if(*(const IDType*)(ptr) > *(const IDType*)(ptr2))
			{
				return true;
			}
			if(*(const IDType*)(ptr) < *(const IDType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::INT:
			if(*(const IntType*)(ptr) > *(const IntType*)(ptr2))
			{
				return true;
			}
			if(*(const IntType*)(ptr) < *(const IntType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::FLOAT:
			if(*(const FloatType*)(ptr) > *(const FloatType*)(ptr2))
			{
				return true;
			}
			if(*(const FloatType*)(ptr) < *(const FloatType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::DATE:
			if(*(const DateType*)(ptr) > *(const DateType*)(ptr2))
			{
				return true;
			}
			if(*(const DateType*)(ptr) < *(const DateType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::VARCHAR:
			if(*(const VarcharType*)(ptr) > *(const VarcharType*)(ptr2))
			{
				return true;
			}
			if(*(const VarcharType*)(ptr) < *(const VarcharType*)(ptr2))
			{
				return false;
			}
			break;
		}
		ptr += len;
		ptr2 += len;
	}
	return false;
}

bool operator <= (PrimKey const& one, PrimKey const& other)
{
    const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	const char* ptr = (const char*)&one;
	const char* ptr2 = (const char*)&other;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
		switch(type)
		{
		case Type::_ID:
			if(*(const IDType*)(ptr) < *(const IDType*)(ptr2))
			{
				return true;
			}
			if(*(const IDType*)(ptr) > *(const IDType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::INT:
			if(*(const IntType*)(ptr) < *(const IntType*)(ptr2))
			{
				return true;
			}
			if(*(const IntType*)(ptr) > *(const IntType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::FLOAT:
			if(*(const FloatType*)(ptr) < *(const FloatType*)(ptr2))
			{
				return true;
			}
			if(*(const FloatType*)(ptr) > *(const FloatType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::DATE:
			if(*(const DateType*)(ptr) < *(const DateType*)(ptr2))
			{
				return true;
			}
			if(*(const DateType*)(ptr) > *(const DateType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::VARCHAR:
			if(*(const VarcharType*)(ptr) < *(const VarcharType*)(ptr2))
			{
				return true;
			}
			if(*(const VarcharType*)(ptr) > *(const VarcharType*)(ptr2))
			{
				return false;
			}
			break;
		}
		ptr += len;
		ptr2 += len;
	}
	return true;
}

bool operator >= (PrimKey const& one, PrimKey const& other)
{
	const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	const char* ptr = (char*)&one;
	const char* ptr2 = (char*)&other;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
		switch(type)
		{
		case Type::_ID:
			if(*(const IDType*)(ptr) > *(const IDType*)(ptr2))
			{
				return true;
			}
			if(*(const IDType*)(ptr) < *(const IDType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::INT:
			if(*(const IntType*)(ptr) > *(const IntType*)(ptr2))
			{
				return true;
			}
			if(*(const IntType*)(ptr) < *(const IntType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::FLOAT:
			if(*(const FloatType*)(ptr) > *(const FloatType*)(ptr2))
			{
				return true;
			}
			if(*(const FloatType*)(ptr) < *(const FloatType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::DATE:
			if(*(const DateType*)(ptr) > *(const DateType*)(ptr2))
			{
				return true;
			}
			if(*(const DateType*)(ptr) < *(const DateType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::VARCHAR:
			if(*(const VarcharType*)(ptr) > *(const VarcharType*)(ptr2))
			{
				return true;
			}
			if(*(const VarcharType*)(ptr) < *(const VarcharType*)(ptr2))
			{
				return false;
			}
			break;
		}
		ptr += len;
		ptr2 += len;
	}
	return true;
}

bool operator != (PrimKey const& one, PrimKey const& other)
{
	return !(one == other);
}

std::ostream& operator << (std::ostream& out, PrimKey const& one)
{
    const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	const char* ptr = (const char*)&one;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
        switch(type)
		{
		case Type::_ID:
			out << *(const IDType*)(ptr);
			break;
		case Type::INT:
			out << *(const IntType*)(ptr);
			break;
		case Type::FLOAT:
			out << *(const FloatType*)(ptr);
			break;
		case Type::DATE:
			out << *(const DateType*)(ptr);
			break;
		case Type::VARCHAR:
			out << *(const VarcharType*)(ptr);
			break;
		}
		ptr += len;
	}
	return out;
}

void operator << (PrimKey& one, PrimKey const& other)
{
	const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	char* ptr = (char*)&one;
	const char* ptr2 = (const char*)&other;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
		copyData(ptr2, ptr, len);
		ptr += len;
		ptr2 += len;
	}
}

bool operator == (PrimKey const& one, PrimKey const& other)
{
	const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	const char* ptr = (const char*)&one;
	const char* ptr2 = (const char*)&other;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
        switch(type)
		{
		case Type::_ID:
			if(*(const IDType*)(ptr) != *(const IDType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::INT:
			if(*(const IntType*)(ptr) != *(const IntType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::FLOAT:
			if(*(const FloatType*)(ptr) != *(const FloatType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::DATE:
			if(*(const DateType*)(ptr) != *(const DateType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::VARCHAR:
			if(*(const VarcharType*)(ptr) != *(const VarcharType*)(ptr2))
			{
				return false;
			}
			break;
		}
		ptr += len;
		ptr2 += len;
	}
	return true;
}

void operator << (VarcharType& one, VarcharType const& other)
{
	strcpy(one.v, other.v);
}

bool operator == (VarcharType const& one, VarcharType const& other)
{
	return strcmp(one.v, other.v) == 0;
}

bool operator != (VarcharType const& one, VarcharType const& other)
{
	return strcmp(one.v, other.v) != 0;
}

bool operator < (VarcharType const& one, VarcharType const& other)
{
	return strcmp(one.v, other.v) < 0;
}

bool operator > (VarcharType const& one, VarcharType const& other)
{
	return strcmp(one.v, other.v) > 0;
}

bool operator <= (VarcharType const& one, VarcharType const& other)
{
	return strcmp(one.v, other.v) <= 0;
}

bool operator >= (VarcharType const& one, VarcharType const& other)
{
	return strcmp(one.v, other.v) >= 0;
}

std::ostream& operator << (std::ostream& out, VarcharType const& one)
{
    out << one.v;
    return out;
}

