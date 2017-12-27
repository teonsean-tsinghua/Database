#! /bin/bash
./build.sh
if [ $? != 0 ] 
then
	exit -1
fi
clear
echo "Starting DBMS..."
./dbms
exit 0
