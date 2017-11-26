#! /bin/bash
cd src
cd DatabaseLayer
echo "Generating lexer and parser files..."
yacc -d parser.y
lex lexer.l
cd ..
echo "Compiling source files..."
make
mv ./application ../dbms
echo "Succeed."
