#include "Connection.hpp"
#include "ServerLog.hpp"

#include <boost/asio.hpp>

namespace web
{
	Connection::Connection(tcp::socket&& con_socket, ConnectionManager& con_manager, RequestHandler& req_handler)
		: socket{ std::move(con_socket) }, connection_manager{ con_manager }, request_handler{ req_handler },
		request_parser{}, buffer{}, timer{ socket.get_io_service() }
	{
		Logger::S_LOG << "New Connection created: " << "[" << remote_endpoint_address() << "]" << std::endl;
	}

	void Connection::start()
	{
		Logger::S_LOG << "Started new Connection: " << "[" << remote_endpoint_address() << "]" << std::endl;
		read();
	}

	void Connection::stop()
	{
		Logger::S_LOG << "Stopped Connection: " << "[" << remote_endpoint_address() << "]" << std::endl;
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
		timer.async_wait([this, this_conn](const boost::system::error_code& ec)
        {
            if(ec)
            {
                return;
            }

            Logger::S_LOG << "Connection timeout: " << "[" << remote_endpoint_address() << "]" << std::endl;
			connection_manager.stop_connection(this_conn);
		});
	}

	void Connection::write(std::unique_ptr<Response> response)
	{
        std::string buffer{
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Length: 44\r\n"
                        "Content-Type: text/html\r\n"
                        "\r\n"
                        "<html><body><h1>It works!</h1></body></html>\r\n"};
        boost::asio::async_write(socket, boost::asio::buffer(buffer),
            [this](boost::system::error_code ec, std::size_t x)
         {
             Logger::S_LOG << "Sending response: " << "[" << remote_endpoint_address() << "]" << std::endl;
             if (!ec)
             {
                 boost::system::error_code ignored_ec;
                 socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
             }

             if (ec != boost::asio::error::operation_aborted)
             {
                 connection_manager.stop_connection(shared_from_this());
             }
         });
    }

}