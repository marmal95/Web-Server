#include "ServerService.hpp"
#include "ServerLog.hpp"

web::ServerService::ServerService(const std::string& address, uint32_t port, const std::string& root_dir)
	: io_service{}, address{ address }, port{ port }, root_dir{ root_dir }, server{ io_service, address, port, root_dir }
{
	Logger::S_LOG << "Initializing ServerService..." << std::endl;
	Logger::S_LOG << "Initialized ServerService." << std::endl;
}

void web::ServerService::start()
{
	Logger::S_LOG << "Starting ServerService..." << std::endl;
	server.start();
}
