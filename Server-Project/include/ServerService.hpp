#pragma once

#include "Server.hpp"

#include <string>
#include <boost/asio/io_service.hpp>


namespace web
{
	using boost::asio::io_service;

	class ServerService
	{
	public:
		ServerService(const std::string& address, uint32_t port);
		void start();

	private:
		io_service io_service;
		Server server;
	};
}