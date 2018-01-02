%{
#include "Include.h"
#include "y.tab.h"

std::string&   replace_all(std::string&   str,const   std::string&   old_value,const   std::string&   new_value)
{
    while(true)   {
        std::string::size_type   pos(0);
        if(   (pos=str.find(old_value))!=std::string::npos   )
            str.replace(pos,old_value.length(),new_value);
        else   break;
    }
    return   str;
}

extern "C"
{
int yywrap(void);
int yylex(void);
}
%}

IDENTIFIER [A-Za-z][_0-9A-Za-z]*
VALUE_INT (-)?[0-9]+
VALUE_FLOAT (-)?[0-9]+(\.[0-9]+)?
VALUE_STRING '((\\')|[^'])*'
SIMPLE_OPERATOR [;,.()=><*+-/]
BLANK_CHARS ([ \f\r\t\v]+)

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
"VARCHAR"		{ return VARCHAR_; }
"DESC"			{ return DESC; }
"INDEX"			{ return INDEX; }
"AND" 			{ return AND; }
"DATE"			{ return DATE_; }
"FLOAT"			{ return FLOAT_; }
"FOREIGN"		{ return FOREIGN; }
"REFERENCES"		{ return REFERENCES; }
"UNIQUE"		{ return UNIQUE; }
"DENSITY"		{ return DENSITY; }
"<>"			{ return NOT_EQUAL; }
"<="			{ return LESS_EQUAL; }
">="			{ return GREATER_EQUAL; }
{SIMPLE_OPERATOR} 	{ return *yytext; }
{IDENTIFIER}    	{ yylval.m_string=yytext; return IDENTIFIER; }
{VALUE_INT}       	{ yylval.m_int=atoi(yytext); return VALUE_INT; }
{VALUE_FLOAT}       	{ yylval.m_float=atof(yytext); return VALUE_FLOAT; }
{VALUE_STRING}    	{ yylval.m_string=yytext; yylval.m_string=replace_all(yylval.m_string, "\\'", "'"); yylval.m_string=replace_all(yylval.m_string, "\\\"", "\""); yylval.m_string=yylval.m_string.substr(1, yylval.m_string.size()-2); return VALUE_STRING; }
{BLANK_CHARS}   	{}
%%

int yywrap(void)
{
    yy_flush_buffer(YY_CURRENT_BUFFER);
    {BEGIN INITIAL;}
return 1;
}
