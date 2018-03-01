#pragma once

#include <memory>

namespace web
{
	struct Response;
	struct Request;

	class IRequestHandler
	{
	public:
		virtual std::unique_ptr<Response> handle_request(std::unique_ptr<Request>) = 0;
		virtual ~IRequestHandler() = default;
	};
}