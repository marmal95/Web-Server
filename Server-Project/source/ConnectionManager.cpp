#include "ConnectionManager.hpp"
#include "Connection.hpp"

namespace web
{
	void ConnectionManager::start_connection(const std::shared_ptr<Connection>& new_connection)
	{
		connections.emplace(new_connection);
		new_connection->start();
	}

	void ConnectionManager::stop_connection(const std::shared_ptr<Connection>& connection)
	{
		connections.erase(connection);
		connection->stop();
	}
}