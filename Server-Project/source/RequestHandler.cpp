#include "RequestHandler.hpp"
#include "Logger.hpp"

web::RequestHandler::RequestHandler(const std::string& root_dir)
	: root_dir{ root_dir }
{
	LOG(INFO) << "Initializing RequestHandler";
	LOG(INFO) << "Initialized RequestHandler";
}

void web::RequestHandler::handle_request(const Request& request)
{
	LOG(INFO) << "Received new Request to handle.";
	LOG(INFO) << "Start handling new Request";
}
