#include"Compare.h"

bool larger(void* a, void* b, int type)
{
    switch(type)
    {
    case Type::PRIMARYKEY:
    {
        return (*(PrimKey*)a > *(PrimKey*)b);
    }
    case Type::_ID:
    {
        return (*(IDType*)a > *(IDType*)b);
    }
    case Type::INT:
    {
        return (*(IntType*)a > *(IntType*)b);
    }
    case Type::FLOAT:
    {
        return (*(FloatType*)a > *(FloatType*)b);
    }
    case Type::DATE:
    {
        return (*(DateType*)a > *(DateType*)b);
    }
    case Type::VARCHAR:
    {
        return (*(VarcharType*)a > *(VarcharType*)b);
    }
    }
    return false;
}

bool smaller(void* a, void* b, int type)
{
    switch(type)
    {
    case Type::PRIMARYKEY:
    {
        return (*(PrimKey*)a < *(PrimKey*)b);
    }
    case Type::_ID:
    {
        return (*(IDType*)a < *(IDType*)b);
    }
    case Type::INT:
    {
        return (*(IntType*)a < *(IntType*)b);
    }
    case Type::FLOAT:
    {
        return (*(FloatType*)a < *(FloatType*)b);
    }
    case Type::DATE:
    {
        return (*(DateType*)a < *(DateType*)b);
    }
    case Type::VARCHAR:
    {
        return (*(VarcharType*)a < *(VarcharType*)b);
    }
    }
    return false;
}

bool Equal(void* a, void* b, int type)
{
    switch(type)
    {
    case Type::PRIMARYKEY:
    {
        return (*(PrimKey*)a == *(PrimKey*)b);
    }
    case Type::_ID:
    {
        return (*(IDType*)a == *(IDType*)b);
    }
    case Type::INT:
    {
        return (*(IntType*)a == *(IntType*)b);
    }
    case Type::FLOAT:
    {
        return (*(FloatType*)a == *(FloatType*)b);
    }
    case Type::DATE:
    {
        return (*(DateType*)a == *(DateType*)b);
    }
    case Type::VARCHAR:
    {
        return (*(VarcharType*)a == *(VarcharType*)b);
    }
    }
    return false;
}

bool largerOrEqual(void* a, void* b, int type)
{
    return !smaller(a, b, type);
}

bool smallerOrEqual(void* a, void* b, int type)
{
    return !larger(a, b, type);
}

