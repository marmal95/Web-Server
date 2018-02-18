#pragma once

#include "Request.hpp"
#include "Response.hpp"
#include "ResponseBuilder.hpp"

#include <memory>
#include <string>
#include <exception>

namespace web
{
	class RequestHandler
	{
	public:
		RequestHandler(const ResponseBuilder& response_builder, std::string_view root_dir);
		RequestHandler(const RequestHandler&) = delete;
		RequestHandler& operator=(const RequestHandler&) = delete;

		std::unique_ptr<IResponse> handle_request(std::unique_ptr<Request> request);

	private:
		const ResponseBuilder& response_builder;
		const std::string root_dir;

		bool is_path_correct(std::string_view path) const;
		bool is_path_dir(std::string_view path) const;
		void append_index(std::string& path);
		std::string get_extension(std::string_view path) const;
		std::string read_file_content(std::string_view path);
	};
}