#pragma once

#include "Server.hpp"

#include <string_view>
#include <boost/asio/io_service.hpp>

namespace web
{
	using boost::asio::io_service;

	class ServerService
	{
	public:
		ServerService(std::string_view address, uint32_t port, std::string_view root_dir);
		void start();

	private:
		io_service service_io;
		std::string address;
		uint32_t port;
		std::string root_dir;

		Server server;
	};
}