/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_HOME_TEON_DOCUMENTS_DBMS_SRC_DATABASELAYER_PARSER_PARSER_HH_INCLUDED
# define YY_YY_HOME_TEON_DOCUMENTS_DBMS_SRC_DATABASELAYER_PARSER_PARSER_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    DATABASE = 258,
    DATABASES = 259,
    TABLE = 260,
    TABLES = 261,
    SHOW = 262,
    CREATE = 263,
    DROP = 264,
    USE = 265,
    PRIMARY = 266,
    KEY = 267,
    NOT = 268,
    NULL_ = 269,
    INSERT = 270,
    INTO = 271,
    VALUES = 272,
    DELETE = 273,
    FROM = 274,
    WHERE = 275,
    UPDATE = 276,
    SET = 277,
    SELECT = 278,
    IS = 279,
    INT_ = 280,
    VARCHAR_ = 281,
    DESC = 282,
    INDEX = 283,
    AND = 284,
    DATE_ = 285,
    FLOAT_ = 286,
    FOREIGN = 287,
    REFERENCES = 288,
    IDENTIFIER = 289,
    VALUE_STRING = 290,
    VALUE_INT = 291,
    NOT_EQUAL = 292,
    GREATER_EQUAL = 293,
    LESS_EQUAL = 294
  };
#endif

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_HOME_TEON_DOCUMENTS_DBMS_SRC_DATABASELAYER_PARSER_PARSER_HH_INCLUDED  */