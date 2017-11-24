#ifndef DBPRINT_H_INCLUDED
#define DBPRINT_H_INCLUDED

#include<string>
#include<iostream>
#include<cstdio>

void DBPrint_ID(char* data);

template<typename T>
void DBLog(const T& msg)
{
     std::cout << msg;
}

template<typename T>
void DBLogLine(const T& msg)
{
    std::cout << msg << std::endl;
}

template<typename T>
void DBPrint(const T& msg)
{
    std::cout << msg;
}

template<typename T>
void DBPrintLine(const T& msg)
{
    std::cout << msg << std::endl;
}

#endif // DBPRINT_H_INCLUDED
