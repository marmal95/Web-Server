#include "Connection.hpp"
#include "ServerLog.hpp"
#include "ResponseEncoder.hpp"
#include "Exception.hpp"

#include <atomic>
#include <boost/asio.hpp>

namespace web
{
	Connection::Connection(tcp::socket&& con_socket, ConnectionManager& con_manager, RequestHandler& req_handler)
		: socket{ std::move(con_socket) }, connection_manager{ con_manager }, request_handler{ req_handler },
		request_parser{}, buffer{}, timer{ socket.get_io_service() }
	{
		static std::atomic<uint32_t> id{};
		conn_id = ++id;
		Logger::S_LOG << "Created new Connection." << " [CONN_ID: " << conn_id << "]" << std::endl;
	}

	void Connection::start()
	{
		Logger::S_LOG << "Started new Connection: " << " [CONN_ID: " << conn_id << "]" << std::endl;
		read();
	}

	void Connection::stop()
	{
		Logger::S_LOG << "Stopped Connection." << " [CONN_ID: " << conn_id << "]" << std::endl;
		socket.shutdown(socket.shutdown_both);
		socket.close();
	}

	uint32_t Connection::get_id() const
	{
		return conn_id;
	}

	void Connection::read()
	{
		auto this_conn{ shared_from_this() };
		socket.async_receive(boost::asio::buffer(buffer),
			[this, this_conn](const boost::system::error_code& ec, std::size_t bytes_transferred)
		{
			stop_timer();
			if (!ec)
			{
				handle_received_data(bytes_transferred);
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

	void Connection::write(std::unique_ptr<Response> response)
	{
		auto this_conn{ shared_from_this() };
		boost::asio::async_write(socket, ResponseEncoder::to_buffers(response),
			[this, this_conn](boost::system::error_code ec, std::size_t)
		{
			Logger::S_LOG << "Sending response..." << " [CONN_ID: " << conn_id << "]" << std::endl;
			if (!ec)
			{
				Logger::S_LOG << "Response sent." << " [CONN_ID: " << conn_id << "]" << std::endl;
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

	void Connection::handle_received_data(size_t bytes_transferred)
	{
		std::unique_ptr<Request> request;
		try
		{
			request = request_parser.parse(boost::asio::buffer(buffer.data(), bytes_transferred));
			handle_received_request(std::move(request));
		}
		catch (const RequestInvalid& exception)
		{
			Logger::S_LOG(InfoLevel::DBG) << "__FILE__" << "#" << __LINE__ <<
				" => " << __FUNCTION__ << "(): An exception occurred: " << exception.what() << std::endl;
			handle_bad_request();
		}
	}

	void Connection::handle_received_request(std::unique_ptr<Request> request)
	{
		Logger::S_LOG << "Request for " << request->uri << " parsed successfully." << std::endl;
		auto response = request_handler.handle_request(std::move(request));
		Logger::S_LOG << "Created response." << std::endl;
		write(std::move(response));
	}

	void Connection::handle_bad_request()
	{
		Logger::S_LOG << "Request parsed unsuccessfully." << std::endl;
		auto response = request_handler.handle_bad_request();
		Logger::S_LOG << "Created response for bad request." << std::endl;
		write(std::move(response));
	}

	void Connection::start_timer()
	{
		Logger::S_LOG << "Timer started." << " [CONN_ID: " << conn_id << "]" << std::endl;
		timer.expires_from_now(boost::posix_time::seconds{ 5 });
	}

	void Connection::set_timeout_handler()
	{
		timer.async_wait([this](const boost::system::error_code& ec)
		{
			if (!ec)
			{
				Logger::S_LOG << "Connection timeout." << " [CONN_ID: " << conn_id << "]" << std::endl;
				close_connection();
			}
		});
	}

	void Connection::stop_timer()
	{
		Logger::S_LOG << "Timer cancelled." << " [CONN_ID: " << conn_id << "]" << std::endl;
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