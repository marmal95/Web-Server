#pragma once

#include "IRequestParser.hpp"
#include "Defs.hpp"
#include "ServerLog.hpp"

#include <iostream>
#include <memory>
#include <utility>

namespace web
{
	struct Request;

	class RequestParser : public IRequestParser
	{
	public:
		std::unique_ptr<Request> parse(const boost::asio::const_buffer& buffer) override;
	};
}