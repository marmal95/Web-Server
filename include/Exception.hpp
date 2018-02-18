#pragma once

#include <exception>
#include <string_view>

class FileNotFound : public std::runtime_error
{
public:
	FileNotFound(std::string_view message);
};