#pragma once

#include "Request.hpp"

#include <memory>

namespace web
{
	class RequestParser
	{
	public:
		RequestParser() = default;
		
		template<typename Iterator>
		std::unique_ptr<Request> parse(Iterator begin, Iterator end);
	};


	template<typename Iterator>
	inline std::unique_ptr<Request> RequestParser::parse(Iterator begin, Iterator end)
	{
		return std::make_unique<Request>();
	}
}