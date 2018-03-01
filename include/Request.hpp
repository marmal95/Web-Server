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
        int http_version_minor;
        int http_version_major;
	};
}