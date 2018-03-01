#pragma once

#include "Defs.hpp"

#include <memory>
#include <boost/asio/buffer.hpp>

namespace web
{
	struct Request;

	class IRequestParser
	{
	public:
		virtual std::unique_ptr<Request> parse(const boost::asio::const_buffer&) = 0;
		virtual ~IRequestParser() = default;
	};
}