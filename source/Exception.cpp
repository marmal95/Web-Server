#include "Exception.hpp"

FileNotFound::FileNotFound(std::string_view message)
	: std::runtime_error{ message.data() }
{}