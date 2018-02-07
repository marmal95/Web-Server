#include "ServerService.hpp"
#include "Logger.hpp"

#include <iostream>


INITIALIZE_EASYLOGGINGPP


int main()
{
	LOG(INFO) << "Starting application.";

	web::ServerService server_service{ "127.0.0.1", 3000, "." };
	server_service.start();

	return EXIT_SUCCESS;
}