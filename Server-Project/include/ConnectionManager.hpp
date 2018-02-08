#pragma once

#include <unordered_set>
#include <memory>

namespace web
{
	class Connection;

	class ConnectionManager
	{
	public:
		ConnectionManager() = default;
		ConnectionManager(const ConnectionManager&) = delete;
		ConnectionManager& operator=(const ConnectionManager&) = delete;

		void start_connection(const std::shared_ptr<Connection>& new_connection);
		void stop_connection(const std::shared_ptr<Connection>& connection);

	private:
		std::unordered_set<std::shared_ptr<Connection>> connections;
	};
}