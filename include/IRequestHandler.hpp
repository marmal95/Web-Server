#pragma once

#include <memory>

namespace web
{
	class IResponse;
	class IRequest;

	class IRequestHandler
	{
	public:
		virtual std::unique_ptr<IResponse> handle_request(std::unique_ptr<IRequest>) = 0;
		virtual ~IRequestHandler() = default;
	};
}