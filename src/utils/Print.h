#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED

#include <iostream>
#include <cstdio>
#include "pagedef.h"

class Print
{
private:

    static Print* instance;

    Print();

public:

    template<typename T>
    static Print& print(const T& msg)
    {
        std::cout << msg;
        return *instance;
    }

    template<typename T>
    static Print& printLine(const T& msg)
    {
        std::cout << msg << std::endl;
        return *instance;
    }

    static Print& printLine()
    {
        std::cout << std::endl;
        return *instance;
    }

    template<typename T>
    static Print& log(const T& msg)
    {
        std::cout << msg;
        return *instance;
    }

    template<typename T>
    static Print& logLine(const T& msg)
    {
        std::cout << msg << std::endl;
        return *instance;
    }

    static Print& logLine()
    {
        std::cout << std::endl;
        return *instance;
    }

    static Print& print_ID(char* data);

};

#endif // PRINT_H_INCLUDED
