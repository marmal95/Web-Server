#pragma once

#include "Header.hpp"
#include "IRequest.hpp"

#include <string>
#include <vector>

namespace web
{
    struct Request : IRequest
	{
        std::vector<Header> headers;
        std::string method;
        std::string uri;
        int http_version_minor;
        int http_version_major;
	};
}