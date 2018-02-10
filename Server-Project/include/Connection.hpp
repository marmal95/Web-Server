#pragma once

#include "ConnectionManager.hpp"
#include "RequestHandler.hpp"
#include "RequestParser.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <array>
#include <boost/asio/ip/tcp.hpp>
#include <memory>

namespace web
{
	using boost::asio::ip::tcp;

	class Connection : public std::enable_shared_from_this<Connection>
	{
	public:
		Connection(tcp::socket&& socket, ConnectionManager& con_manager, RequestHandler& req_handler);
		Connection(const Connection&) = delete;
		Connection& operator=(const Connection&) = delete;

		void start();
		void stop();
		std::string remote_endpoint_address() const;

	private:
		tcp::socket socket;
		ConnectionManager& connection_manager;
		RequestHandler& request_handler;
		RequestParser request_parser;
		std::array<char, 8192> buffer;

		void read();
		void write();
	};
}