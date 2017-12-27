#include"Type.h"
#include<cstdio>
#include"CacheIOModel.h"
#include"Info.h"

RecordInfo* PrimKey::ri;

const int Type::typeSize_[] = {16, sizeof(int), sizeof(float), 8, 0};

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

bool operator == (IDType& one, IDType& other)
{
	return one.ts == other.ts && one.ca == other.ca && one.pid == other.pid && one.rd == other.rd;
}

void operator << (IDType& one, IDType& other)
{
	one.ts = other.ts;
	one.ca = other.ca;
	one.pid = other.pid;
	one.rd = other.rd;
}

bool operator != (IDType& one, IDType& other)
{
	return !(one == other);
}

bool operator < (IDType& one, IDType& other)
{
	return one.ts < other.ts ||
			(one.ts == other.ts && one.ca < other.ca) ||
			(one.ts == other.ts && one.ca == other.ca && one.pid < other.pid) ||
			(one.ts == other.ts && one.ca == other.ca && one.pid == other.pid && one.rd < other.rd);
}

bool operator > (IDType& one, IDType& other)
{
	return one.ts > other.ts ||
			(one.ts == other.ts && one.ca > other.ca) ||
			(one.ts == other.ts && one.ca == other.ca && one.pid > other.pid) ||
			(one.ts == other.ts && one.ca == other.ca && one.pid == other.pid && one.rd > other.rd);
}

bool operator <= (IDType& one, IDType& other)
{
	return !(one > other);
}

bool operator >= (IDType& one, IDType& other)
{
	return !(one < other);
}

bool operator == (IntType& one, IntType& other)
{
	return one.v == other.v;
}

bool operator != (IntType& one, IntType& other)
{
	return one.v != other.v;
}

bool operator < (IntType& one, IntType& other)
{
	return one.v < other.v;
}

bool operator > (IntType& one, IntType& other)
{
	return one.v > other.v;
}

void operator << (IntType& one, IntType& other)
{
	one.v = other.v;
}

bool operator <= (IntType& one, IntType& other)
{
	return one.v <= other.v;
}

bool operator >= (IntType& one, IntType& other)
{
	return one.v >= other.v;
}

bool operator == (FloatType& one, FloatType& other)
{
	return one.v == other.v;
}

bool operator != (FloatType& one, FloatType& other)
{
	return one.v != other.v;
}

bool operator < (FloatType& one, FloatType& other)
{
	return one.v < other.v;
}

bool operator > (FloatType& one, FloatType& other)
{
	return one.v > other.v;
}

void operator << (FloatType& one, FloatType& other)
{
	one.v = other.v;
}

bool operator <= (FloatType& one, FloatType& other)
{
	return one.v <= other.v;
}

bool operator >= (FloatType& one, FloatType& other)
{
	return one.v >= other.v;
}

bool operator == (DateType& one, DateType& other)
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

bool operator != (DateType& one, DateType& other)
{
	return !(one == other);
}

void operator << (DateType& one, DateType& other)
{
	strncpy(one.v, other.v, 8);
}

bool operator < (DateType& one, DateType& other)
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

bool operator > (DateType& one, DateType& other)
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

bool operator <= (DateType& one, DateType& other)
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

bool operator >= (DateType& one, DateType& other)
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

bool operator < (PrimKey& one, PrimKey& other)
{
	const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	char* ptr = (char*)one.keys;
	char* ptr2 = (char*)other.keys;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
		switch(type)
		{
		case Type::_ID:
			if(*(IDType*)(ptr) < *(IDType*)(ptr2))
			{
				return true;
			}
			if(*(IDType*)(ptr) > *(IDType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::INT:
			if(*(IntType*)(ptr) < *(IntType*)(ptr2))
			{
				return true;
			}
			if(*(IntType*)(ptr) > *(IntType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::FLOAT:
			if(*(FloatType*)(ptr) < *(FloatType*)(ptr2))
			{
				return true;
			}
			if(*(FloatType*)(ptr) > *(FloatType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::DATE:
			if(*(DateType*)(ptr) < *(DateType*)(ptr2))
			{
				return true;
			}
			if(*(DateType*)(ptr) > *(DateType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::VARCHAR:
			if(*(VarcharType*)(ptr) < *(VarcharType*)(ptr2))
			{
				return true;
			}
			if(*(VarcharType*)(ptr) > *(VarcharType*)(ptr2))
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

bool operator > (PrimKey& one, PrimKey& other)
{
	const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	char* ptr = (char*)one.keys;
	char* ptr2 = (char*)other.keys;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
		switch(type)
		{
		case Type::_ID:
			if(*(IDType*)(ptr) > *(IDType*)(ptr2))
			{
				return true;
			}
			if(*(IDType*)(ptr) < *(IDType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::INT:
			if(*(IntType*)(ptr) > *(IntType*)(ptr2))
			{
				return true;
			}
			if(*(IntType*)(ptr) < *(IntType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::FLOAT:
			if(*(FloatType*)(ptr) > *(FloatType*)(ptr2))
			{
				return true;
			}
			if(*(FloatType*)(ptr) < *(FloatType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::DATE:
			if(*(DateType*)(ptr) > *(DateType*)(ptr2))
			{
				return true;
			}
			if(*(DateType*)(ptr) < *(DateType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::VARCHAR:
			if(*(VarcharType*)(ptr) > *(VarcharType*)(ptr2))
			{
				return true;
			}
			if(*(VarcharType*)(ptr) < *(VarcharType*)(ptr2))
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

bool operator <= (PrimKey& one, PrimKey& other)
{
    const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	char* ptr = (char*)one.keys;
	char* ptr2 = (char*)other.keys;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
		switch(type)
		{
		case Type::_ID:
			if(*(IDType*)(ptr) < *(IDType*)(ptr2))
			{
				return true;
			}
			if(*(IDType*)(ptr) > *(IDType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::INT:
			if(*(IntType*)(ptr) < *(IntType*)(ptr2))
			{
				return true;
			}
			if(*(IntType*)(ptr) > *(IntType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::FLOAT:
			if(*(FloatType*)(ptr) < *(FloatType*)(ptr2))
			{
				return true;
			}
			if(*(FloatType*)(ptr) > *(FloatType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::DATE:
			if(*(DateType*)(ptr) < *(DateType*)(ptr2))
			{
				return true;
			}
			if(*(DateType*)(ptr) > *(DateType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::VARCHAR:
			if(*(VarcharType*)(ptr) < *(VarcharType*)(ptr2))
			{
				return true;
			}
			if(*(VarcharType*)(ptr) > *(VarcharType*)(ptr2))
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

bool operator >= (PrimKey& one, PrimKey& other)
{
	const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	char* ptr = (char*)one.keys;
	char* ptr2 = (char*)other.keys;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
		switch(type)
		{
		case Type::_ID:
			if(*(IDType*)(ptr) > *(IDType*)(ptr2))
			{
				return true;
			}
			if(*(IDType*)(ptr) < *(IDType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::INT:
			if(*(IntType*)(ptr) > *(IntType*)(ptr2))
			{
				return true;
			}
			if(*(IntType*)(ptr) < *(IntType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::FLOAT:
			if(*(FloatType*)(ptr) > *(FloatType*)(ptr2))
			{
				return true;
			}
			if(*(FloatType*)(ptr) < *(FloatType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::DATE:
			if(*(DateType*)(ptr) > *(DateType*)(ptr2))
			{
				return true;
			}
			if(*(DateType*)(ptr) < *(DateType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::VARCHAR:
			if(*(VarcharType*)(ptr) > *(VarcharType*)(ptr2))
			{
				return true;
			}
			if(*(VarcharType*)(ptr) < *(VarcharType*)(ptr2))
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

bool operator != (PrimKey& one, PrimKey& other)
{
	return !(one == other);
}

void operator << (PrimKey& one, PrimKey& other)
{
	const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	char* ptr = (char*)one.keys;
	char* ptr2 = (char*)other.keys;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
		copyData(ptr2, ptr, len);
		ptr += len;
		ptr2 += len;
	}
}

bool operator == (PrimKey& one, PrimKey& other)
{
	const std::vector<int>& info = PrimKey::ri->getPrimKeyInfo();
	char* ptr = (char*)one.keys;
	char* ptr2 = (char*)other.keys;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];
        switch(type)
		{
		case Type::_ID:
			if(*(IDType*)(ptr) != *(IDType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::INT:
			if(*(IntType*)(ptr) != *(IntType*)(ptr2))
			{
				return true;
			}
			break;
		case Type::FLOAT:
			if(*(FloatType*)(ptr) != *(FloatType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::DATE:
			if(*(DateType*)(ptr) != *(DateType*)(ptr2))
			{
				return false;
			}
			break;
		case Type::VARCHAR:
			if(*(VarcharType*)(ptr) != *(VarcharType*)(ptr2))
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

void operator << (VarcharType& one, VarcharType& other)
{
	strcpy(one.v, other.v);
}

bool operator == (VarcharType& one, VarcharType& other)
{
	return strcmp(one.v, other.v) == 0;
}

bool operator != (VarcharType& one, VarcharType& other)
{
	return strcmp(one.v, other.v) != 0;
}

bool operator < (VarcharType& one, VarcharType& other)
{
	return strcmp(one.v, other.v) < 0;
}

bool operator > (VarcharType& one, VarcharType& other)
{
	return strcmp(one.v, other.v) > 0;
}

bool operator <= (VarcharType& one, VarcharType& other)
{
	return strcmp(one.v, other.v) <= 0;
}

bool operator >= (VarcharType& one, VarcharType& other)
{
	return strcmp(one.v, other.v) >= 0;
}

