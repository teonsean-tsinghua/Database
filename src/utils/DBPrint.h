#ifndef DBPRINT_H_INCLUDED
#define DBPRINT_H_INCLUDED

#include <iostream>
#include <cstdio>
#include "pagedef.h"

class DBPrint
{
private:

    static DBPrint* instance;

    DBPrint();

public:

    template<typename T>
    static DBPrint& print(const T& msg)
    {
        std::cout << msg;
        return *instance;
    }

    template<typename T>
    static DBPrint& printLine(const T& msg)
    {
        std::cout << msg << std::endl;
        return *instance;
    }

    static DBPrint& printLine()
    {
        std::cout << std::endl;
        return *instance;
    }

    template<typename T>
    static DBPrint& log(const T& msg)
    {
        std::cout << msg;
        return *instance;
    }

    template<typename T>
    static DBPrint& logLine(const T& msg)
    {
        std::cout << msg << std::endl;
        return *instance;
    }

    static DBPrint& logLine()
    {
        std::cout << std::endl;
        return *instance;
    }

    static DBPrint& print_ID(char* data);

};

#endif // DBPRINT_H_INCLUDED
