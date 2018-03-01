#pragma once

#include "Defs.hpp"
#include "Response.hpp"
#include <vector>
#include <memory>
#include <boost/asio/buffer.hpp>

namespace web
{
	class ResponseEncoder
	{
	public:
		static boost::asio::const_buffer to_buffer(ResponseStatus response_status);
		static std::vector<boost::asio::const_buffer> to_buffers(const std::unique_ptr<Response>& response);
	};
}