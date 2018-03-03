#pragma once

#include "Header.hpp"

#include <string>
#include <vector>

namespace web
{
    struct Request
	{
        std::vector<Header> headers;
        std::string method;
        std::string uri;
		std::string http_ver;
	};
}