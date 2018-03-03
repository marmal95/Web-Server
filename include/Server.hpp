#pragma once

#include "IServer.hpp"
#include "RequestHandler.hpp"
#include "ConnectionManager.hpp"
#include "ResponseBuilder.hpp"
#include "RequestParser.hpp"

#include <string>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace web
{
	using namespace boost::asio;
	using namespace boost::asio::ip;
	
	class Server : public IServer
	{
	public:
		Server(std::string_view address, uint32_t port, std::string_view root_dir);
		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;

		void start() override;
		void stop() override;
		void reset() override;

	private:
		io_service service_io;
		tcp::acceptor tcp_acceptor;
		tcp::socket tcp_socket;
		ResponseBuilder response_builder;
		RequestHandler request_handler;
		RequestParser request_parser;
		ConnectionManager connection_manager;

		void accept();
	};

}
