#pragma once

#include "Request.hpp"
#include "Response.hpp"

#include <memory>
#include <string>

namespace web
{
	class RequestHandler
	{
	public:
		RequestHandler(const std::string& root_dir);
		RequestHandler(const RequestHandler&) = delete;
		RequestHandler& operator=(const RequestHandler&) = delete;

		std::unique_ptr<Response> handle_request(std::unique_ptr<Request> request);

	private:
		std::string root_dir;
	};
}