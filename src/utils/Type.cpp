#include"Type.h"
#include<cstdio>
#include"Info.h"

const int Type::typeSize_[] = {16, sizeof(int), sizeof(float), 8, 0};

const char* const Type::typeName_[] = {"_ID", "INT", "FLOAT", "DATE", "VARCHAR"};

const char* const Type::pageName_[] = {"Data file Desc page",
									   "Data page",
									   "Usage page",
							  		   "Index file description page",
									   "Index internal page",
									   "Index leaf page",
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

int IDType::type()
{
	return Type::_ID;
}

bool IDType::operator == (DataType& other) const
{
	return ts == IDType(other).ts && ca == IDType(other).ca && pid == IDType(other).pid && rd == IDType(other).rd;
}

bool IDType::operator != (DataType& other) const
{
	return !((*this) == IDType(other));
}

bool IDType::operator < (DataType& other) const
{
	return ts < IDType(other).ts ||
			(ts == IDType(other).ts && ca < IDType(other).ca) ||
			(ts == IDType(other).ts && ca == IDType(other).ca && pid < IDType(other).pid) ||
			(ts == IDType(other).ts && ca == IDType(other).ca && pid == IDType(other).pid && rd < IDType(other).rd);
}

bool IDType::operator > (DataType& other) const
{
	return ts > IDType(other).ts ||
			(ts == IDType(other).ts && ca > IDType(other).ca) ||
			(ts == IDType(other).ts && ca == IDType(other).ca && pid > IDType(other).pid) ||
			(ts == IDType(other).ts && ca == IDType(other).ca && pid == IDType(other).pid && rd > IDType(other).rd);
}

bool IDType::operator <= (DataType& other) const
{
	return !((*this) > IDType(other));
}

bool IDType::operator >= (DataType& other) const
{
	return !((*this) < IDType(other));
}

int IntType::type()
{
	return Type::INT;
}

bool IntType::operator == (DataType& other) const
{
	return v == ((IntType)other).v
}

bool IntType::operator != (DataType& other) const
{
	return v != ((IntType)other).v
}

bool IntType::operator < (DataType& other) const
{
	return v < ((IntType)other).v
}

bool IntType::operator > (DataType& other) const
{
	return v > ((IntType)other).v
}

bool IntType::operator <= (DataType& other) const
{
	return v <= ((IntType)other).v
}

bool IntType::operator >= (DataType& other) const
{
	return v >= ((IntType)other).v
}

int FloatType::type()
{
	return Type::FLOAT;
}

bool FloatType::operator == (DataType& other) const
{
	return v == ((FloatType)other).v;
}

bool FloatType::operator != (DataType& other) const
{
	return v != ((FloatType)other).v;
}

bool FloatType::operator < (DataType& other) const
{
	return v < ((FloatType)other).v;
}

bool FloatType::operator > (DataType& other) const
{
	return v > ((FloatType)other).v;
}

bool FloatType::operator <= (DataType& other) const
{
	return v <= ((FloatType)other).v;
}

bool FloatType::operator >= (DataType& other) const
{
	return v >= ((FloatType)other).v;
}

int DateType::type()
{
	return Type::DATE;
}

bool DateType::operator == (DataType& other) const
{
	for(int i = 0; i < 8; i++)
	{
		if(v[i] != ((DateType)other).v[i])
		{
			return false;
		}
	}
	return true;
}

bool DateType::operator != (DataType& other) const
{
	return !((*this) == ((DateType)other));
}

bool DateType::operator < (DataType& other) const
{
	for(int i = 0; i < 8; i++)
	{
		if(v[i] < ((DateType)other).v[i])
		{
			return true;
		}
		else if(v[i] > ((DateType)other).v[i])
		{
			return false;
		}
	}
	return false;
}

bool DateType::operator > (DataType& other) const
{
	for(int i = 0; i < 8; i++)
	{
		if(v[i] < ((DateType)other).v[i])
		{
			return false;
		}
		else if(v[i] > ((DateType)other).v[i])
		{
			return true;
		}
	}
	return false;
}

bool DateType::operator <= (DataType& other) const
{

	for(int i = 0; i < 8; i++)
	{
		if(v[i] < ((DateType)other).v[i])
		{
			return true;
		}
		else if(v[i] > ((DateType)other).v[i])
		{
			return false;
		}
	}
	return true;
}

bool DateType::operator >= (DataType& other) const
{
	for(int i = 0; i < 8; i++)
	{
		if(v[i] < ((DateType)other).v[i])
		{
			return false;
		}
		else if(v[i] > ((DateType)other).v[i])
		{
			return true;
		}
	}
	return true;
}

RecordInfo* PrimKey::ri;

void PrimKey::setRecordInfo(RecordInfo* ri)
{
	this->ri = ri;
}

bool PrimKey::operator < (DataType& other) const
{
	const std::vector<int>& info = ri->getPrimKeyInfo();
	char* ptr = (char*)keys;
	char* ptr2 = (char*)((PrimKey)other).keys;
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

bool PrimKey::operator > (DataType& other) const
{

}

bool PrimKey::operator <= (DataType& other) const
{

}

bool PrimKey::operator >= (DataType& other) const
{

}

bool PrimKey::operator != (DataType& other) const
{
	return !((*this) == ((PrimKey)other));
}

bool PrimKey::operator == (DataType& other) const
{
	const std::vector<int>& info = ri->getPrimKeyInfo();
	char* ptr = (char*)keys;
	char* ptr2 = (char*)((PrimKey)other).keys;
	for(int i = 0; i < info.size();)
	{
		int type = info[i++];
		int len = info[i++];

		ptr += len;
		ptr2 += len;
	}
	return true;
}

int VarcharType::type()
{
	return Type::VARCHAR;
}

bool VarcharType::operator == (DataType& other) const
{
	return strcmp(v, ((VarcharType)other).v) == 0;
}

bool VarcharType::operator != (DataType& other) const
{
	return strcmp(v, ((VarcharType)other).v) != 0;
}

bool VarcharType::operator < (DataType& other) const
{
	return strcmp(v, ((VarcharType)other).v) < 0;
}

bool VarcharType::operator > (DataType& other) const
{
	return strcmp(v, ((VarcharType)other).v) > 0;
}

bool VarcharType::operator <= (DataType& other) const
{
	return strcmp(v, ((VarcharType)other).v) <= 0;
}

bool VarcharType::operator >= (DataType& other) const
{
	return strcmp(v, ((VarcharType)other).v) >= 0;
}

