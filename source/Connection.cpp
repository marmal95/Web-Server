#include "Connection.hpp"
#include "ServerLog.hpp"
#include <boost/asio.hpp>

namespace web
{
	Connection::Connection(tcp::socket&& con_socket, ConnectionManager& con_manager, RequestHandler& req_handler)
		: socket{ std::move(con_socket) }, connection_manager{ con_manager }, request_handler{ req_handler },
		request_parser{}, buffer{}, timer{ socket.get_io_service() }
	{
		Logger::S_LOG << "Created new Connection: " << "[" << remote_endpoint_address() << "]" << std::endl;
	}

	void Connection::start()
	{
		Logger::S_LOG << "Started new Connection: " << "[" << remote_endpoint_address() << "]" << std::endl;
		read();
	}

	void Connection::stop()
	{
		Logger::S_LOG << "Stopped Connection: " << "[" << remote_endpoint_address() << "]" << std::endl;
		socket.shutdown(socket.shutdown_both);
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
				auto request = request_parser.parse(boost::asio::buffer(buffer.data(), bytes_transferred));
				handle_parsed_request(std::move(request));
			}
			else
			{
				Logger::S_LOG(InfoLevel::DBG) << "__FILE__" << "#" << __LINE__ <<
					" => " << __FUNCTION__ << "(): Following error occured: " << ec.message() <<
					" (error code: " << ec << ")" << std::endl;
				close_connection_if_not_aborted(ec);
			}
		});

		start_timer();
		set_timeout_handler();
	}

	void Connection::write(std::unique_ptr<IResponse> response)
	{
		auto this_conn{ shared_from_this() };
		boost::asio::async_write(socket, response->to_buffers(),
			[this, this_conn](boost::system::error_code ec, std::size_t bytes_transferred)
		{
			Logger::S_LOG << "Sending response: " << "[" << remote_endpoint_address() << "]" << std::endl;
			if (!ec)
			{
				Logger::S_LOG << "Response sent to: [" << remote_endpoint_address() << "]" << std::endl;
				boost::system::error_code ignored_ec;
				socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
				close_connection();
			}
			else
			{
				Logger::S_LOG(InfoLevel::DBG) << "__FILE__" << "#" << __LINE__ <<
					" => " << __FUNCTION__ << "(): An error occurred: " << ec.message() <<
					" (error code: " << ec << ")" << std::endl;
				close_connection_if_not_aborted(ec);
			}
		});
	}

	void Connection::handle_parsed_request(std::pair<ResultType, std::unique_ptr<Request>> parsed_req)
	{
		if (parsed_req.first == ResultType::good)
		{
			handle_good_request(std::move(parsed_req.second));
		}
		else
		{
			handle_bad_request(std::move(parsed_req.second));
		}
	}

	void Connection::handle_good_request(std::unique_ptr<Request> request)
	{
		Logger::S_LOG << "Request for " << request->uri << " parsed successfully." << std::endl;
		auto response = request_handler.handle_request(std::move(request));
		Logger::S_LOG << "Created response." << std::endl;
		write(std::move(response));
	}

	void Connection::handle_bad_request(std::unique_ptr<Request> request)
	{
		Logger::S_LOG(InfoLevel::DBG) << "__FILE__" << "#" << __LINE__ <<
			" => " << __FUNCTION__ << "(): Error occurred while parsing request." << std::endl;
		request.reset();
	}

	void Connection::start_timer()
	{
		Logger::S_LOG << "Timer for: " << "[" << remote_endpoint_address() << "]" << " started." << std::endl;;
		timer.expires_from_now(boost::posix_time::seconds{ 5 });
	}

	void Connection::set_timeout_handler()
	{
		timer.async_wait([this](const boost::system::error_code& ec)
		{
			if (!ec)
			{
				Logger::S_LOG << "Connection timeout: " << "[" << remote_endpoint_address() << "]" << std::endl;
				close_connection();
			}
		});
	}

	void Connection::stop_timer()
	{
		Logger::S_LOG << "Timer for: " << "[" << remote_endpoint_address() << "]" << " stopped." << std::endl;
		timer.cancel();
	}

	void Connection::close_connection()
	{
		connection_manager.stop_connection(shared_from_this());
	}

	void Connection::close_connection_if_not_aborted(const boost::system::error_code& ec)
	{
		if (ec != boost::asio::error::operation_aborted)
		{
			close_connection();
		}
	}
}