#pragma once

#include <stdexcept>
#include <string_view>

class FileNotFound : public std::runtime_error
{
public:
	FileNotFound(std::string_view message);
};