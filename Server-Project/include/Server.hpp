#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace web
{
	using namespace boost::asio;
	using namespace boost::asio::ip;
	
	class Server
	{
	public:
		Server(const std::shared_ptr<io_service>& io_service);
		void start();

	private:
		std::shared_ptr<io_service> io_service;
		tcp::endpoint tcp_endpoint;
		tcp::acceptor tcp_acceptor;
		tcp::socket tcp_socket;

		void accept();
	};

}
