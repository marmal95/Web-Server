#include "Server.hpp"
#include "Logger.hpp"

#include <iostream>
#include <boost/asio/io_service.hpp>

INITIALIZE_EASYLOGGINGPP

int main()
{
	LOG(INFO) << "Initializing Server";
	auto io_service = std::make_shared<boost::asio::io_service>();
	
	web::Server server{io_service};
	server.start();

	LOG(INFO) << "Exiting Server";
	return EXIT_SUCCESS;
}