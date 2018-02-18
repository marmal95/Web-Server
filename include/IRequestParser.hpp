#pragma once

#include <memory>

namespace web
{
	struct Request;
	using Request_uPtr = std::unique_ptr<Request>;

	class IRequestParser
	{
	public:
		// [TODO]:
		virtual ~IRequestParser() = default;
	};
}