#include"Exception.h"

Exception::Exception(std::string tag, std::string msg):
    tag(tag), msg(msg)
{

}

bool Exception::compare(std::string str)
{
	return str == msg;
}

void Exception::print()
{
    std::cout << tag << ": " << msg << std::endl;
}
