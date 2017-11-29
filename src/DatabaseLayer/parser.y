%{
#include "DBBase.h"
#include "DBDatabase.h"

DBDataBase* instance = DBDataBase::getInstance();

extern "C"
{
    void yyerror(const char *s);
    extern int yylex(void);
}

%}
%token	DATABASE	DATABASES	TABLE	TABLES	SHOW	CREATE
%token	DROP	USE	PRIMARY	KEY	NOT	NULL_
%token  INSERT	INTO	VALUES	DELETE	FROM	WHERE
%token  UPDATE	SET	SELECT	IS	INT_	VARCHAR_
%token  DESC	INDEX 	AND	DATE_	FLOAT_	FOREIGN	REFERENCES
%token  <m_string>IDENTIFIER
%token  <m_string>VALUE_STRING
%token	<m_int>VALUE_INT
%token  NOT_EQUAL GREATER_EQUAL LESS_EQUAL
%token  ';' 	'('	')' 	','	'.' '<' '>' '=' '*'

%type<m_string> dbName tbName colName
%type<m_type> type

%nonassoc ';'
%left AND
%%

program	: program stmt
	| /* empty */
	;

stmt	: sysStmt ';'
	| dbStmt ';'
	| tbStmt ';'
	| idxStmt ';'
	;

sysStmt	: SHOW DATABASES { instance->showDatabases(); }
	;

dbStmt	: CREATE DATABASE dbName { instance->createDatabase($3.c_str()); }
	| DROP DATABASE dbName { instance->dropDatabase($3.c_str()); }
	| USE dbName { instance->useDatabase($2.c_str()); }
	| SHOW TABLES { instance->showTables(); }
	;

tbStmt	: CREATE TABLE tbName '(' fieldList ')'
	| DROP TABLE tbName { instance->dropTable($3.c_str()); }
	| DESC tbName
	| INSERT INTO tbName VALUES valueLists
	| DELETE FROM tbName WHERE whereClause
	| UPDATE tbName SET setClause WHERE whereClause
	| SELECT selector FROM tableList WHERE whereClause
	;

idxStmt	: CREATE INDEX tbName '(' colName ')'
	| DROP INDEX tbName '(' colName ')'
	;

fieldList : field
	| fieldList ',' field
	;

field	: colName type {  }
	| colName type NOT NULL_
	| PRIMARY KEY '(' columnList ')'
	| FOREIGN KEY '(' colName ')' REFERENCES tbName '(' colName ')'
	;

type	: INT_ '(' VALUE_INT ')' { $$.type = DBType::INT; $$.extra = $3; }
    | INT_ { $$.type = DBType::INT; $$.extra = 0; }
	| VARCHAR_ '(' VALUE_INT ')' { $$.type = DBType::VARCHAR; $$.extra = $3; }
	| DATE_ { $$.type = DBType::DATE; $$.extra = 0; }
	| FLOAT_ { $$.type = DBType::FLOAT; $$.extra = 0; }
	;

valueLists : '(' valueList ')'
	| valueLists ',' '(' valueList ')'
	;

valueList : value
	| valueList ',' value
	;

value	: VALUE_INT
	| VALUE_STRING
	| NULL_
	;

whereClause : col op expr
	| col IS NULL_
	| col IS NOT NULL_
	| whereClause AND whereClause %prec AND
	;

col	: tbName '.' colName
	| colName
	;

op	: '='
	| NOT_EQUAL
	| LESS_EQUAL
	| GREATER_EQUAL
	| '<'
	| '>'
	;

expr	: value
	| col
	;

setClause : colName '=' value
	| setClause ',' colName '=' value
	;

selector : '*'
	| colList
	;

colList : colList ',' col
	| col
	;

tableList : tbName
	| tableList ',' tbName
	;

columnList : colName
	| columnList ',' colName
	;

dbName	: IDENTIFIER { $$ = $1; }
	;

tbName	: IDENTIFIER { $$ = $1; }
	;

colName	: IDENTIFIER { $$ = $1; }
	;

%%

void yyerror(const char *s)
{
    cerr<<s<<endl;
}

