#pragma once

#include <exception>
#include <string>

class Exception : std::exception
{
public:
	Exception(const std::string& message);
	const char* what() const override;

protected:
	std::string message;
};