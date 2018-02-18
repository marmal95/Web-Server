#pragma once

#include "IResponse.hpp"
#include "Header.hpp"
#include "Defs.hpp"

#include <vector>
#include <string>
#include <memory>
#include <boost/asio/buffer.hpp>

namespace web
{
	struct Response : IResponse
	{
		std::vector<Header> headers;
		std::string content;
		ResponseStatus status;
		
		std::vector<boost::asio::const_buffer> to_buffers() const override;
	};
}