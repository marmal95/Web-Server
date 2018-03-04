#pragma once

#include "Server.hpp"

#include <string_view>

namespace web
{
	class ServerService
	{
	public:
		ServerService(std::string_view root_dir, std::string_view address, uint32_t port);
		void start();

	private:
		std::string root_dir;
		std::string address;
		uint32_t port;

		Server server;
	};
}