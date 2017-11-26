%{
#include "DBBase.h"//lex和yacc要共用的头文件，里面包含了一些头文件，重定义了YYSTYPE
#include "y.tab.h"//用yacc编译yacc.y后生成的C头文件，内含%token、YYSTYPE、yylval等定义(都是C宏)，供lex.yy.c和yacc.tab.c使用

extern "C"//为了能够在C++程序里面调用C函数，必须把每一个需要使用的C函数，其声明都包括在extern "C"{}块里面，这样C++链接时才能成功链接它们。extern "C"用来在C++环境下设置C链接类型。
{   //yacc.y中也有类似的这段extern "C"，可以把它们合并成一段，放到共同的头文件main.h中
int yywrap(void);
int yylex(void);//这个是lex生成的词法分析函数，yacc的yyparse()里会调用它，如果这里不声明，生成的yacc.tab.c在编译时会找不到该函数
}
%}

IDENTIFIER [A-Za-z][_0-9A-Za-z]*
VALUE_INT [0-9]+
VALUE_STRING '[^']*'
SIMPLE_OPERATOR [;,.()=><*]
BLANK_CHARS ([ \f\r\t\v]+)

/*下面%%后开始第2段：规则段*/
%%
"DATABASE"		{ return DATABASE; }
"DATABASES"		{ return DATABASES; }
"TABLE"			{ return TABLE; }
"TABLES"		{ return TABLES; }
"SHOW"			{ return SHOW; }
"CREATE"		{ return CREATE; }
"DROP"			{ return DROP; }
"USE"			{ return USE; }
"PRIMARY"		{ return PRIMARY; }
"KEY"			{ return KEY; }
"NOT"			{ return NOT; }
"NULL"			{ return NULL_; }
"INSERT"		{ return INSERT; }
"INTO"			{ return INTO; }
"VALUES"		{ return VALUES; }
"DELETE"		{ return DELETE; }
"FROM"			{ return FROM; }
"WHERE"			{ return WHERE; }
"UPDATE"		{ return UPDATE; }
"SET"			{ return SET; }
"SELECT"		{ return SELECT; }
"IS"			{ return IS; }
"INT"			{ return INT_; }
"VARCHAR"		{ return VARCHAR; }
"DESC"			{ return DESC; }
"INDEX"			{ return INDEX; }
"AND" 			{ return AND; }
"DATE"			{ return DATE; }
"FLOAT"			{ return FLOAT; }
"FOREIGN"		{ return FOREIGN; }
"REFERENCES"		{ return REFERENCES; }
"<>"			{ return NOT_EQUAL; }
"<="			{ return LESS_EQUAL; }
">="			{ return GREATER_EQUAL; }
{SIMPLE_OPERATOR} 	{ return *yytext; }
{IDENTIFIER}    	{ yylval.m_sId=yytext; return IDENTIFIER; }
{VALUE_INT}       	{ yylval.m_nInt=atoi(yytext); return VALUE_INT; }
{VALUE_STRING}    	{ yylval.m_sId=yytext; return VALUE_STRING; }
{BLANK_CHARS}   	{}
%%

//第3段：C函数定义段
int yywrap(void)
{
puts("-----the file is end");
return 1;//返回1表示读取全部结束。如果要接着读其它文件，可以这里fopen该文件，文件指针赋给yyin，并返回0
}


