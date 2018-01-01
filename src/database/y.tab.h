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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    UNIQUE = 270,
    INSERT = 271,
    INTO = 272,
    VALUES = 273,
    DELETE = 274,
    FROM = 275,
    WHERE = 276,
    DENSITY = 277,
    UPDATE = 278,
    SET = 279,
    SELECT = 280,
    IS = 281,
    INT_ = 282,
    VARCHAR_ = 283,
    DESC = 284,
    INDEX = 285,
    AND = 286,
    DATE_ = 287,
    FLOAT_ = 288,
    FOREIGN = 289,
    REFERENCES = 290,
    IDENTIFIER = 291,
    VALUE_STRING = 292,
    VALUE_INT = 293,
    NOT_EQUAL = 294,
    GREATER_EQUAL = 295,
    LESS_EQUAL = 296
  };
#endif
/* Tokens.  */
#define DATABASE 258
#define DATABASES 259
#define TABLE 260
#define TABLES 261
#define SHOW 262
#define CREATE 263
#define DROP 264
#define USE 265
#define PRIMARY 266
#define KEY 267
#define NOT 268
#define NULL_ 269
#define UNIQUE 270
#define INSERT 271
#define INTO 272
#define VALUES 273
#define DELETE 274
#define FROM 275
#define WHERE 276
#define DENSITY 277
#define UPDATE 278
#define SET 279
#define SELECT 280
#define IS 281
#define INT_ 282
#define VARCHAR_ 283
#define DESC 284
#define INDEX 285
#define AND 286
#define DATE_ 287
#define FLOAT_ 288
#define FOREIGN 289
#define REFERENCES 290
#define IDENTIFIER 291
#define VALUE_STRING 292
#define VALUE_INT 293
#define NOT_EQUAL 294
#define GREATER_EQUAL 295
#define LESS_EQUAL 296

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
