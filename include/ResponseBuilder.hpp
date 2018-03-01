#pragma once

#include "IResponseBuilder.hpp"

#include <map>

namespace web
{
	class ResponseBuilder : public IResponseBuilder
	{
	public:
		ResponseBuilder();
		std::unique_ptr<Response> build(ResponseStatus status, std::optional<std::string_view> content, std::optional<std::string_view> extension) const override;

	private:
		std::map<ResponseStatus, std::string> status_str_responses;
		std::map<std::string, std::string> extensions;

		void fill_status_str_responses();
		void fill_extensions_map();
	};
}