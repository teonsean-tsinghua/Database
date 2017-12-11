#! /bin/bash
cd src
cd DatabaseLayer
echo "============================================"
echo "Generating lexer and parser files..."
yacc -d parser.y
if [ $? != 0 ] 
then
	echo "============================================"
	echo "Failed."
	exit
fi
lex lexer.l
if [ $? != 0 ] 
then
	echo "============================================"
	echo "Failed."
	exit
fi
cd ..
echo "Succeeded."
echo "============================================"
echo "Compiling source files..."
make
if [ $? != 0 ] 
then
	echo "============================================"
	echo "Failed."
	exit
fi
mv ./application ../dbms
echo "============================================"
echo "Succeed."
