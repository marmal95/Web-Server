#include "Exception.hpp"

Exception::Exception(const std::string& message)
	: message{ message }
{}

const char* Exception::what() const
{
	return message.c_str();
}
