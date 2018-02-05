#include <iostream>
#include "Logger.hpp"

INITIALIZE_EASYLOGGINGPP

int main()
{
	LOG(INFO) << "Initializing Server";
	LOG(INFO) << "Exiting Server";
	return EXIT_SUCCESS;
}