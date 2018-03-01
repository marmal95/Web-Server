#pragma once

#include "Header.hpp"
#include "Defs.hpp"
#include <vector>
#include <string>
#include <memory>
#include <boost/asio/buffer.hpp>

namespace web
{
	struct Response
	{
		std::vector<Header> headers;
		std::string content;
		ResponseStatus status;
	};
}