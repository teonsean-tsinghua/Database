#! /bin/bash
cd src
cd database
echo "============================================"
echo "Generating lexer and parser files..."
yacc -d parser.y
if [ $? != 0 ] 
then
	echo "============================================"
	echo "Build failed."
	exit -1
fi
lex lexer.l
if [ $? != 0 ] 
then
	echo "============================================"
	echo "Build failed."
	exit -1
fi
cd ..
echo "Succeeded."
echo "============================================"
echo "Compiling source files..."
make
if [ $? != 0 ] 
then
	echo "============================================"
	echo "Build failed."
	exit -1 
fi
mv ./application ../dbms
echo "============================================"
echo "Succeed."
exit 0
