#include"DBException.h"

Exception::Exception(std::string msg):
    msg(msg)
{

}

void Exception::print()
{
    DBPrint::printLine(msg);
}
