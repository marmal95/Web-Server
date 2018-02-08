#include "Connection.hpp"
#include "Logger.hpp"

#include <iostream>

namespace web
{
	Connection::Connection(tcp::socket&& socket, ConnectionManager& con_manager, RequestHandler& req_handler)
		: socket{ std::move(socket) }, connection_manager{ con_manager }, request_handler{ req_handler }, request_parser{}, buffer{}
	{}

	void Connection::start()
	{
		LOG(INFO) << "Started new Connection.";
		read();
	}

	void Connection::stop()
	{
		LOG(INFO) << "Stopped Connection.";
	}

	void Connection::read()
	{
		auto this_conn{ shared_from_this() };
		socket.async_read_some(boost::asio::buffer(buffer),
			[this, this_conn](const boost::system::error_code& ec, std::size_t bytes_transferred)
		{
			if (!ec)
			{
				auto buffer_data = buffer.data();
				auto request = request_parser.parse(buffer_data, buffer_data + bytes_transferred);
				auto response = request_handler.handle_request(std::move(request));
			}
			else if (ec != boost::asio::error::operation_aborted)
			{
				connection_manager.stop_connection(shared_from_this());
			}
			else
			{
				// Log error
			}
		});
	}

	void Connection::write()
	{
	}

}