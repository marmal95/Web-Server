#include "ServerService.hpp"
#include "ServerLog.hpp"

web::ServerService::ServerService(std::string_view root_dir, std::string_view address, uint32_t port)
	: address{ address }, port{ port }, root_dir{ root_dir }, server{ address, port, root_dir }
{
	Logger::S_LOG << "Initializing ServerService..." << std::endl;
	Logger::S_LOG << "Initialized ServerService." << std::endl;
}

void web::ServerService::start()
{
	Logger::S_LOG << "Starting ServerService..." << std::endl;
	server.start();
}
