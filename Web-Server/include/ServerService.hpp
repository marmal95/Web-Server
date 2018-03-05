#pragma once

#include "Server.hpp"
#include "IServerFactory.hpp"
#include <string_view>
#include <memory>

namespace web
{
    class IServer;

	class ServerService
	{
	public:
		ServerService(std::unique_ptr<IServerFactory> server_factory);
		void start();

	private:
		std::string root_dir;
		std::string address;
		uint32_t port;

        std::unique_ptr<IServer> server;
	};
}
