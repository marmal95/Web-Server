#include "ServerService.hpp"
#include "Logger.hpp"

web::ServerService::ServerService(const std::string& address, uint32_t port, const std::string& root_dir)
	: io_service{}, address{ address }, port{ port }, root_dir{ root_dir }, server{ io_service, address, port, root_dir }
{
	LOG(INFO) << "Initializing ServerService.";
	LOG(INFO) << "Initialized ServerService.";
}

void web::ServerService::start()
{
	LOG(INFO) << "Starting ServerService.";
	server.start();
}
