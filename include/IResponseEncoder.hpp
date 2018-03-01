#pragma once

#include "Defs.hpp"

#include <vector>
#include <memory>
#include <boost/asio/buffer.hpp>

namespace web
{
	struct Response;
	using asio_buffer = boost::asio::const_buffer;
	using asio_buffers = std::vector<boost::asio::const_buffer>;

	class IResponseEncoder
	{
	public:
		virtual asio_buffer to_buffer(ResponseStatus response_status) const = 0;
		virtual asio_buffers to_buffers(const std::unique_ptr<Response>& response) const = 0;
		virtual ~IResponseEncoder() = default;
	};
}