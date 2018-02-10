#include "Connection.hpp"
#include "ServerLog.hpp"

#include <iostream>

namespace web
{
	Connection::Connection(tcp::socket&& con_socket, ConnectionManager& con_manager, RequestHandler& req_handler)
		: socket{ std::move(con_socket) }, connection_manager{ con_manager }, request_handler{ req_handler }, request_parser{}, buffer{}
	{
		Logger::S_LOG << "New Connection created: " << "[" << socket.remote_endpoint().address().to_string() << "]" << std::endl;
	}

	void Connection::start()
	{
		Logger::S_LOG << "Started new Connection: " << "[" << socket.remote_endpoint().address().to_string() << "]" << std::endl;
		read();
	}

	void Connection::stop()
	{
		Logger::S_LOG << "Stopped Connection: " << "[" << socket.remote_endpoint().address().to_string() << "]" << std::endl;
		socket.close();
	}

	std::string Connection::remote_endpoint_address() const
	{
		return socket.remote_endpoint().address().to_string();
	}

	void Connection::read()
	{
		//auto this_conn{ shared_from_this() };
		socket.async_read_some(boost::asio::buffer(buffer),
			[this](const boost::system::error_code& ec, std::size_t bytes_transferred)
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
				Logger::S_LOG(InfoLevel::ERR) << __FILE__ << "#" << __LINE__ << ":" << __FUNCTION__ << "Error while reading from Connection." << std::endl;
			}
		});
	}

	void Connection::write()
	{
	}

}