#pragma once

#include "RequestHandler.hpp"

#include <string>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace web
{
	using namespace boost::asio;
	using namespace boost::asio::ip;
	
	class Server
	{
	public:
		Server(io_service& io_service, const std::string& address, uint32_t port, const std::string& root_dir);
		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;

		void start();

	private:
		io_service& io_service;
		tcp::acceptor tcp_acceptor;
		tcp::socket tcp_socket;
		RequestHandler request_handler;

		void accept();
	};

}
