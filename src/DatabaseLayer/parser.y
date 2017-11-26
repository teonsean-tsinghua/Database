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
%token  UPDATE	SET	SELECT	IS	INT_	VARCHAR
%token  DESC	INDEX 	AND	DATE	FLOAT	FOREIGN	REFERENCES
%token  <m_sId>IDENTIFIER
%token  <m_sId>VALUE_STRING
%token	<m_nInt>VALUE_INT
%token  NOT_EQUAL GREATER_EQUAL LESS_EQUAL
%token  ';' 	'('	')' 	','	'.' '<' '>' '=' '*'

%type<m_sId>dbName tbName colName

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

sysStmt	: SHOW DATABASES
	;

dbStmt	: CREATE DATABASE dbName { instance->createDatabase($3.c_str()); }
	| DROP DATABASE dbName
	| USE dbName
	| SHOW TABLES
	;

tbStmt	: CREATE TABLE tbName '(' fieldList ')'
	| DROP TABLE tbName
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

field	: colName type
	| colName type NOT NULL_
	| PRIMARY KEY '(' columnList ')'
	| FOREIGN KEY '(' colName ')' REFERENCES tbName '(' colName ')'
	;

type	: INT_ '(' VALUE_INT ')'
	| VARCHAR '(' VALUE_INT ')'
	| DATE
	| FLOAT
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

