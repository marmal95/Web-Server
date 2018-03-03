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
		std::vector<std::string> split_first_line(const std::vector<std::string>& lines) const;
		template<typename Func>
		void throw_exception_if_condition_fail(Func condition_func) const;
	};

	template<typename Func>
	inline void RequestParser::throw_exception_if_condition_fail(Func condition_func) const
	{
		if (condition_func())
		{
			throw RequestInvalid{ "Invalid Request. Could not parse." };
		}
	}
}