#include"Compare.h"

bool larger(const void* a, const void* b, int type)
{
    switch(type)
    {
    case Type::PRIMARYKEY:
    {
        return (*(const PrimKey*)a > *(const PrimKey*)b);
    }
    case Type::_ID:
    {
        return (*(const IDType*)a > *(const IDType*)b);
    }
    case Type::INT:
    {
        return (*(const IntType*)a > *(const IntType*)b);
    }
    case Type::FLOAT:
    {
        return (*(const FloatType*)a > *(const FloatType*)b);
    }
    case Type::DATE:
    {
        return (*(const DateType*)a > *(const DateType*)b);
    }
    case Type::VARCHAR:
    {
        return (*(const VarcharType*)a > *(const VarcharType*)b);
    }
    }
    return false;
}

bool smaller(const void* a, const void* b, int type)
{
    switch(type)
    {
    case Type::PRIMARYKEY:
    {
        return (*(const PrimKey*)a < *(const PrimKey*)b);
    }
    case Type::_ID:
    {
        return (*(const IDType*)a < *(const IDType*)b);
    }
    case Type::INT:
    {
        return (*(const IntType*)a < *(const IntType*)b);
    }
    case Type::FLOAT:
    {
        return (*(const FloatType*)a < *(const FloatType*)b);
    }
    case Type::DATE:
    {
        return (*(const DateType*)a < *(const DateType*)b);
    }
    case Type::VARCHAR:
    {
        return (*(const VarcharType*)a < *(const VarcharType*)b);
    }
    }
    return false;
}

bool Equal(const void* a, const void* b, int type)
{
    switch(type)
    {
    case Type::PRIMARYKEY:
    {
        return (*(const PrimKey*)a == *(const PrimKey*)b);
    }
    case Type::_ID:
    {
        return (*(const IDType*)a == *(const IDType*)b);
    }
    case Type::INT:
    {
        return (*(const IntType*)a == *(const IntType*)b);
    }
    case Type::FLOAT:
    {
        return (*(const FloatType*)a == *(const FloatType*)b);
    }
    case Type::DATE:
    {
        return (*(const DateType*)a == *(const DateType*)b);
    }
    case Type::VARCHAR:
    {
        return (*(const VarcharType*)a == *(const VarcharType*)b);
    }
    }
    return false;
}

bool largerOrEqual(const void* a, const void* b, int type)
{
    return !smaller(a, b, type);
}

bool smallerOrEqual(const void* a, const void* b, int type)
{
    return !larger(a, b, type);
}

