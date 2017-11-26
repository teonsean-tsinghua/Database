#! /bin/bash
cd src
cd DatabaseLayer
yacc -d parser.y
lex lexer.l
cd ..
make
mv ./application ../dbms
rm *.o
