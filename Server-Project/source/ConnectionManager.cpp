#include "ConnectionManager.hpp"
#include "Connection.hpp"
#include "ServerLog.hpp"

namespace web
{
	void ConnectionManager::start_connection(const std::shared_ptr<Connection>& new_connection)
	{
		Logger::S_LOG << "Starting new Connection: " << "[" << new_connection->remote_endpoint_address() << "]" << std::endl;
		connections.emplace(new_connection);
		new_connection->start();
	}

	void ConnectionManager::stop_connection(const std::shared_ptr<Connection>& connection)
	{
		Logger::S_LOG << "Stopping Connecion: " << "[" << connection->remote_endpoint_address() << "]" << std::endl;
		connections.erase(connection);
		connection->stop();
	}
}