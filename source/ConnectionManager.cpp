#include "ConnectionManager.hpp"
#include "Connection.hpp"
#include "ServerLog.hpp"

namespace web
{
	void ConnectionManager::start_connection(const std::shared_ptr<IConnection>& new_connection)
	{
		Logger::S_LOG << "Starting new Connection..." << " [CONN_ID: " << new_connection->get_id() << "]" << std::endl;
		connections.emplace(new_connection);
		new_connection->start();
	}

	void ConnectionManager::stop_connection(const std::shared_ptr<IConnection>& connection)
	{
		Logger::S_LOG << "Stopping Connection..." << " [CONN_ID: " << connection->get_id() << "]" << std::endl;
		connection->stop();
		connections.erase(connection);
	}
}