#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED

#define assertException(stmt, msg)	bool thrown = false;					\
									try										\
									{										\
										stmt;								\
									}										\
									catch(Exception& e)						\
									{										\
										assert(e.compare(msg));				\
										e.print();							\
										thrown = true;						\
									}										\
									assert(thrown)							\

#include<string>
#include<iostream>

class Exception
{
    std::string msg;
    std::string tag;

public:
    Exception(std::string tag, std::string msg);

    bool compare(std::string str);

    void print();
};

#endif // EXCEPTION_H_INCLUDED
