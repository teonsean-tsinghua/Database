%{
#include "DBBase.h"
#include "DBDatabase.h"

DBDatabase* instance = DBDatabase::getInstance();

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
%type<m_field> field
%type<m_value> value
%type<m_col> col
%type<m_bool> selector

%nonassoc ';'
%left AND
%%

program	: program stmt {}
	| /* empty */ {}
	;

stmt	: sysStmt ';' {}
	| dbStmt ';' {}
	| tbStmt ';' {}
	| idxStmt ';' {}
	;

sysStmt	: SHOW DATABASES { instance->showDatabases(); }
	;

dbStmt	: CREATE DATABASE dbName { instance->createDatabase($3); }
	| DROP DATABASE dbName { instance->dropDatabase($3); }
	| USE dbName { instance->useDatabase($2); }
	| SHOW TABLES { instance->showTables(); }
	;

tbStmt	: CREATE TABLE tbName '(' fieldList ')' { instance->createTable($3); }
	| DROP TABLE tbName { instance->dropTable($3); }
	| DESC tbName { instance->describeTable($2); }
	| INSERT INTO tbName VALUES valueLists { instance->insert($3); }
	| DELETE FROM tbName WHERE whereClause
	| UPDATE tbName SET setClause WHERE whereClause
	| SELECT selector FROM tableList WHERE whereClause { instance->select($2); } // unfinished
	;

idxStmt	: CREATE INDEX tbName '(' colName ')'
	| DROP INDEX tbName '(' colName ')'
	;

fieldList : field { instance->addPendingField($1.name, $1.type, $1.nullable, $1.extra); }
	| fieldList ',' field { instance->addPendingField($3.name, $3.type, $3.nullable, $3.extra); }
	;

field	: colName type { $$.type = $2.type; $$.extra = $2.extra; $$.nullable = true; $$.name = $1; }
	| colName type NOT NULL_ { $$.type = $2.type; $$.extra = $2.extra; $$.nullable = false; $$.name = $1; }
	| PRIMARY KEY '(' columnList ')'
	| FOREIGN KEY '(' colName ')' REFERENCES tbName '(' colName ')'
	;

type	: INT_ '(' VALUE_INT ')' { $$.type = DBType::INT; $$.extra = $3; }
    | INT_ { $$.type = DBType::INT; $$.extra = 0; }
	| VARCHAR_ '(' VALUE_INT ')' { $$.type = DBType::VARCHAR; $$.extra = $3; }
	| DATE_ { $$.type = DBType::DATE; $$.extra = 0; }
	| FLOAT_ { $$.type = DBType::FLOAT; $$.extra = 0; }
	;

valueLists : '(' valueList ')' { instance->addPendingValueList(); }
	| valueLists ',' '(' valueList ')' { instance->addPendingValueList(); }
	;

valueList : value { instance->addPendingValue($1); }
	| valueList ',' value { instance->addPendingValue($3); }
	;

value	: VALUE_INT { $$.type = 1; $$.v_int = $1; }
	| VALUE_STRING { $$.type = 2; $$.v_str = $1; }
	| NULL_ { $$.type = 0; }
	;

whereClause : col op expr
	| col IS NULL_
	| col IS NOT NULL_
	| whereClause AND whereClause %prec AND
	;

col	: tbName '.' colName { $$.table = $1; $$.field = $3; }
	| colName { $$.table = ""; $$.field = $1; }
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

selector : '*' { $$ = true; }
	| colList { $$ = false; }
	;

colList : colList ',' col { instance->addPendingCol($3); }
	| col { instance->addPendingCol($1); }
	;

tableList : tbName { instance->addPendingTable($1); }
	| tableList ',' tbName { instance->addPendingTable($3); }
	;

columnList : colName { instance->addPendingColumn($1); }
	| columnList ',' colName { instance->addPendingColumn($3); }
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

