#ifndef DBPRINT_H_INCLUDED
#define DBPRINT_H_INCLUDED

#include<string>
#include<iostream>

void DBLog(std::string msg);

void DBLogLine(std::string msg = "");

void DBPrint(std::string msg);

void DBPrintLine(std::string msg = "");

void DBLog(int msg);

void DBLogLine(int msg);

void DBPrint(int msg);

void DBPrintLine(int msg);

#endif // DBPRINT_H_INCLUDED
