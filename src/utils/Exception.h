#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED

#include<string>
#include"Print.h"

class Exception
{
    std::string msg;

public:
    Exception(std::string msg);

    virtual void print();
};

#endif // EXCEPTION_H_INCLUDED
