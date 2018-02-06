#include "ServerService.hpp"
#include "Logger.hpp"

web::ServerService::ServerService(const std::string& address, uint32_t port)
	: io_service{}, server{ io_service, address, port }
{
	LOG(INFO) << "Initializing ServerService.";
	LOG(INFO) << "Initialized ServerService.";
}

void web::ServerService::start()
{
	LOG(INFO) << "Starting ServerService.";
	server.start();
}
