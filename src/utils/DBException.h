#ifndef DBEXCEPTION_H_INCLUDED
#define DBEXCEPTION_H_INCLUDED

#include<string>
#include"DBPrint.h"

class Exception
{
    std::string msg;

public:
    Exception(std::string msg);

    virtual void print();
};

#endif // DBEXCEPTION_H_INCLUDED
