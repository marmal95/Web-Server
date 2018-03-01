#pragma once

#include "IConnection.hpp"
#include "ConnectionManager.hpp"
#include "RequestHandler.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"

#include <array>
#include <memory>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/deadline_timer.hpp>

namespace web
{
	using boost::asio::ip::tcp;

	class Connection : public IConnection, public std::enable_shared_from_this<Connection>
	{
	public:
		Connection(tcp::socket&& socket, ConnectionManager& con_manager, RequestHandler& req_handler);
		Connection(const Connection&) = delete;
		Connection& operator=(const Connection&) = delete;

		void start() override;
		void stop() override;
		uint32_t get_id() const override;

	private:
		tcp::socket socket;
		ConnectionManager& connection_manager;
		RequestHandler& request_handler;
		RequestParser request_parser;
		std::array<char, 8192> buffer;
		boost::asio::deadline_timer timer;
		uint32_t conn_id;

		void read();
		void write(std::unique_ptr<Response> response);
		void handle_parsed_request(std::pair<ResultType, std::unique_ptr<Request>> parsed_req);
		void handle_good_request(std::unique_ptr<Request> request);
		void handle_bad_request(std::unique_ptr<Request> request);
		void start_timer();
		void set_timeout_handler();
		void stop_timer();
		void close_connection();
		void close_connection_if_not_aborted(const boost::system::error_code& ec);
	};
}