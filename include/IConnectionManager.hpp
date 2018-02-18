#pragma once

#include <memory>

namespace web
{
	class IConnection;

	class IConnectionManager
	{
	public:
		virtual void start_connection(const std::shared_ptr<IConnection>&) = 0;
		virtual void stop_connection(const std::shared_ptr<IConnection>&) = 0;
		virtual ~IConnectionManager() = default;
	};
}