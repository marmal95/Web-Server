#include "ServerService.hpp"
#include "ServerFactory.hpp"

int main()
{
	Logger::S_LOG << "Starting application..." << std::endl;

	web::ServerService server_service{ std::make_unique<web::ServerFactory>("web", "127.0.0.1", 3000) };
	server_service.start();

	Logger::S_LOG << "Exiting application..." << std::endl;
	return EXIT_SUCCESS;
}
