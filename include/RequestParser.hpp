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
	using Request_uPtr = std::unique_ptr<Request>;

	class RequestParser : public IRequestParser
	{
	public:
		RequestParser();
		std::pair<ResultType, Request_uPtr> parse(const boost::asio::const_buffer& buffer) override;
		void reset();

	private:
		std::unique_ptr<Request> request;
		ParserState state;

		ResultType consume(char input);
		bool is_special(int c) const;
	};
}