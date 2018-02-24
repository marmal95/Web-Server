#pragma once

#include "Defs.hpp"
#include <memory>
#include <boost/asio/buffer.hpp>

namespace web
{
	struct Request;
	using Request_uPtr = std::unique_ptr<Request>;

	class IRequestParser
	{
	public:
		virtual std::pair<ResultType, Request_uPtr> parse(const boost::asio::const_buffer&) = 0;
		virtual ~IRequestParser() = default;
	};
}