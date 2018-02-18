#pragma once

#include <vector>
#include <boost/asio/buffer.hpp>

namespace web
{
	class IResponse
	{
	public:
		virtual std::vector<boost::asio::const_buffer> to_buffers() const = 0;
		virtual ~IResponse() = default;
	};
}