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
		ServerService(const std::string& address, uint32_t port, const std::string& root_dir);
		void start();

	private:
		io_service service_io;
		std::string address;
		uint32_t port;
		std::string root_dir;

		Server server;
	};
}