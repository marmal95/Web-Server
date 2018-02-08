#include "Connection.hpp"
#include "Logger.hpp"

namespace web
{
	Connection::Connection(tcp::socket&& socket, ConnectionManager& con_manager, RequestHandler& req_handler)
		: socket{ std::move(socket) }, connection_manager{ con_manager }, request_handler{ req_handler }
	{}

	void Connection::start()
	{
		LOG(INFO) << "Started new Connection.";
	}

	void Connection::stop()
	{
		LOG(INFO) << "Stopped Connection.";
	}

}