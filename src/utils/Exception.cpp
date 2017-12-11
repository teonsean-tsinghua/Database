#include"Exception.h"

Exception::Exception(std::string msg):
    msg(msg)
{

}

void Exception::print()
{
    Print::printLine(msg);
}
