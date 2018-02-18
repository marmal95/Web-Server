#pragma once

#include <string>

namespace web
{
	class IConnection
	{
	public:
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual std::string remote_endpoint_address() const = 0;
		virtual ~IConnection() = default;
	};
}