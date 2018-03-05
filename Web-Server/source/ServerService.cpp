#include "ServerService.hpp"

web::ServerService::ServerService(std::unique_ptr<IServerFactory> server_factory)
	: server { server_factory->create() }
{
	Logger::S_LOG << "Initializing ServerService..." << std::endl;
	Logger::S_LOG << "Initialized ServerService." << std::endl;
}

void web::ServerService::start()
{
	Logger::S_LOG << "Starting ServerService..." << std::endl;
	server->start();
}
