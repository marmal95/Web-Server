#include "Connection.hpp"
#include "ServerLog.hpp"

#include <iostream>
#include <boost/asio/deadline_timer.hpp>

namespace web
{
	Connection::Connection(tcp::socket&& con_socket, ConnectionManager& con_manager, RequestHandler& req_handler)
		: socket{ std::move(con_socket) }, connection_manager{ con_manager }, request_handler{ req_handler },
		request_parser{}, buffer{}, timer{ socket.get_io_service() }
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
		auto this_conn{ shared_from_this() };
		socket.async_receive(boost::asio::buffer(buffer),
			[this, this_conn](const boost::system::error_code& ec, std::size_t bytes_transferred)
		{
			timer.cancel();
			if (!ec)
			{
				auto buffer_data = buffer.data();
				auto request = request_parser.parse(buffer_data, buffer_data + bytes_transferred);
				auto response = request_handler.handle_request(std::move(request));
			}
			else if (ec != boost::asio::error::operation_aborted)
			{
				//connection_manager.stop_connection(this_conn); // [BUG]: Crash happens
			}
			else
			{
				Logger::S_LOG(InfoLevel::ERR) << __FILE__ << "#" << __LINE__ << ":" << __FUNCTION__ << "Error while reading from Connection." << std::endl;
				connection_manager.stop_connection(this_conn);
			}
		});

		timer.expires_from_now(boost::posix_time::seconds{ 5 });
		timer.async_wait([this, this_conn](const boost::system::error_code& ec)
		{
			Logger::S_LOG << "Connection timeout: " << "[" << socket.remote_endpoint().address().to_string() << "]" << std::endl;
			connection_manager.stop_connection(this_conn); 
		});
	}

	void Connection::write()
	{}

}