#pragma once

#include "Defs.hpp"
#include <memory>
#include <string_view>
#include <optional>


namespace web
{
	class IResponse;

	class IResponseBuilder
	{
	public:
		virtual std::unique_ptr<IResponse> build(
			ResponseStatus status, std::optional<std::string_view> content, std::optional<std::string_view> extension) const = 0;
		virtual ~IResponseBuilder() = default;
	};
}