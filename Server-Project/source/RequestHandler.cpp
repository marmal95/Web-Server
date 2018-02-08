#include "RequestHandler.hpp"
#include "Logger.hpp"

namespace web
{
	RequestHandler::RequestHandler(const std::string& root_dir)
		: root_dir{ root_dir }
	{
		LOG(INFO) << "Initializing RequestHandler";
		LOG(INFO) << "Initialized RequestHandler";
	}

	std::unique_ptr<Response> RequestHandler::handle_request(std::unique_ptr<Request> request)
	{
		LOG(INFO) << "Received new Request to handle.";
		LOG(INFO) << "Start handling new Request";
		return std::make_unique<Response>();
	}
}
