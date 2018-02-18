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
		std::string remote_endpoint_address() const;

	private:
		tcp::socket socket;
		ConnectionManager& connection_manager;
		RequestHandler& request_handler;
		RequestParser request_parser;
		std::array<char, 8192> buffer;
		boost::asio::deadline_timer timer;

		void read();
		void write(std::unique_ptr<IResponse> response);
	};
}