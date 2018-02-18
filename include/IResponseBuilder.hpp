#pragma once

#include "Defs.hpp"
#include <memory>
#include <string>


namespace web
{
	class IResponse;

	class IResponseBuilder
	{
	public:
		virtual std::unique_ptr<IResponse> build(ResponseStatus status, const std::string& content, const std::string& extension) const = 0;
		virtual ~IResponseBuilder() = default;
	};
}