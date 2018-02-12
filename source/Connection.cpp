#include "Connection.hpp"
#include "ServerLog.hpp"

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
                write(std::move(response));
			}
			else if (ec != boost::asio::error::operation_aborted)
			{
                Logger::S_LOG(InfoLevel::DBG) << ec << std::endl;
				connection_manager.stop_connection(this_conn);
			}
			else
			{
				Logger::S_LOG(InfoLevel::DBG) << __FILE__ << "#" << __LINE__ << " Error occurred while reading data. Code error: " << ec << std::endl;
			}
		});

		timer.expires_from_now(boost::posix_time::seconds{ 5 });
		timer.async_wait([this, this_conn](const boost::system::error_code &ec)
        {
            if(ec)
            {
                return;
            }

            Logger::S_LOG << "Connection timeout: " << "[" << socket.remote_endpoint().address().to_string() << "]" << std::endl;
			connection_manager.stop_connection(this_conn);
		});
	}

	void Connection::write(std::unique_ptr<Response> response)
	{
        std::array<char, 8192> buffer{"Elo"};
        socket.async_write_some(boost::asio::buffer(buffer),
            [this](const boost::system::error_code& ec, auto x)
            {
                Logger::S_LOG << "Sent." << std::endl;
            });
    }

}