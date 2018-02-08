#pragma once

#include "ConnectionManager.hpp"
#include "RequestHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <array>
#include <boost/asio/ip/tcp.hpp>

namespace web
{
	using boost::asio::ip::tcp;

	class Connection
	{
	public:
		Connection(tcp::socket&& socket, ConnectionManager& con_manager, RequestHandler& req_handler);
		Connection(const Connection&) = delete;
		Connection& operator=(const Connection&) = delete;

		void start();
		void stop();

	private:
		tcp::socket socket;
		ConnectionManager& connection_manager;
		RequestHandler& request_handler;
		std::array<char, 8192> buffer;

		Request request;
		Response response;
	};
}