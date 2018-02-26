#pragma once

#include <string>

namespace web
{
	class IConnection
	{
	public:
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual uint32_t get_id() const = 0;
		virtual ~IConnection() = default;
	};
}