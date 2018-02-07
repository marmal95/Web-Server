#pragma once

#include "Request.hpp"
#include <string>

namespace web
{
	class RequestHandler
	{
	public:
		RequestHandler(const std::string& root_dir);
		RequestHandler(const RequestHandler&) = delete;
		RequestHandler& operator=(const RequestHandler&) = delete;

		void handle_request(const Request& request);

	private:
		std::string root_dir;
	};
}