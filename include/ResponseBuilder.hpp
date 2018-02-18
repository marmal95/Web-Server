#pragma once

#include "IResponseBuilder.hpp"
#include <map>
#include <string>

namespace web
{
	class ResponseBuilder : public IResponseBuilder
	{
	public:
		ResponseBuilder();
		std::unique_ptr<IResponse> build(ResponseStatus status, const std::string& content, const std::string& extension = "html") const override;

	private:
		std::map<ResponseStatus, std::string> status_str_responses;
		std::map<std::string, std::string> extensions;

		void fill_status_str_responses();
		void fill_extensions_map();
	};
}