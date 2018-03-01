#pragma once

#include "IConnectionManager.hpp"

#include <unordered_set>
#include <memory>

namespace web
{
	class IConnection;

	class ConnectionManager : public IConnectionManager
	{
	public:
		ConnectionManager() = default;
		ConnectionManager(const ConnectionManager&) = delete;
		ConnectionManager& operator=(const ConnectionManager&) = delete;

		void start_connection(const std::shared_ptr<IConnection>& new_connection) override;
		void stop_connection(const std::shared_ptr<IConnection>& connection) override;

	private:
		std::unordered_set<std::shared_ptr<IConnection>> connections;
	};
}