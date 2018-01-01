#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>

struct Field
{
    int type;
    int extra;
    bool nullable;
    std::string name;
};

struct FieldType
{
    int type;
    int extra;
};

struct Value
{
    int type; // 0-null, 1-int, 2-str
    int v_int;
    std::string v_str;
    float v_float;
};

struct SetRValue
{
    int type;
    Value value;
    std::string col;
    std::string col2;
};

struct Col
{
    std::string table;
    std::string field;
};

struct Set
{
    std::string field;
    struct SetRValue rvalue;
};

struct Foreign
{
	std::string field;
	std::string table;
	std::string column;
};

struct Where
{
    int type; // 0-is null, 1-not null, 2-op
    int op; // 0-=, 1-<=, 2->=, 3-<, 4->, 5-<>
    bool opCol;
    Col left;
    Col col_r;
    Value val_r;
};

struct Type_
{
	std::string m_string;
	int m_int;
	float m_float;
	bool m_bool;
	struct Field m_field;
	struct FieldType m_type;
	struct Value m_value;
	struct Col m_col;
	struct Where m_where;
	struct Set m_set;
	struct SetRValue m_rvalue;
};

#define YYSTYPE Type_

#endif // BASE_H_INCLUDED
