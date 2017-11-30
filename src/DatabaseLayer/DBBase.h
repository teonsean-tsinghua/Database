#ifndef DBBASE_H_INCLUDED
#define DBBASE_H_INCLUDED

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
};

struct Type
{
	std::string m_string;
	int m_int;
	struct Field m_field;
	struct FieldType m_type;
	struct Value m_value;
};

#define YYSTYPE Type

#endif // DBBASE_H_INCLUDED
