#include "RequestHandler.hpp"
#include "ServerLog.hpp"

namespace web
{
	RequestHandler::RequestHandler(const std::string& root_dir)
		: root_dir{ root_dir }
	{
		Logger::S_LOG << "Initializing RequestHandler..." << std::endl;
		Logger::S_LOG << "Initialized RequestHandler." << std::endl;
	}

	std::unique_ptr<Response> RequestHandler::handle_request(std::unique_ptr<Request> request)
	{
		Logger::S_LOG << "Received new Request to handle." << std::endl;
		Logger::S_LOG << "Start handling new Request..." << std::endl;
		return std::make_unique<Response>();
	}
}
