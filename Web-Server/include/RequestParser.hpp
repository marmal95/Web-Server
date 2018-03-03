#pragma once

#include "IRequestParser.hpp"
#include "Defs.hpp"
#include "ServerLog.hpp"

#include <iostream>
#include <memory>
#include <utility>
#include <functional>

namespace web
{
	struct Request;

	class RequestParser : public IRequestParser
	{
	public:
		std::unique_ptr<Request> parse(const boost::asio::const_buffer& buffer) override;

	private:
		std::vector<std::string> split_lines(std::string_view content) const;
		std::vector<std::string> split_request_line(const std::vector<std::string>& lines) const;
		template<typename Func>
		void conditionally_throw_exception(Func condition_func) const;
	};

	template<typename Func>
	inline void RequestParser::conditionally_throw_exception(Func condition_func) const
	{
		if (condition_func())
		{
			throw RequestInvalid{ "Invalid Request. Could not parse." };
		}
	}
}