#pragma once

#include "Request.hpp"

#include <iostream>
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
		auto iter = begin;
		while (begin++ < end)
		{
			std::cout << *begin;
		}
		return std::make_unique<Request>();
	}
}