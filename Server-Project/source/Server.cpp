#include "Server.hpp"
#include "Logger.hpp"
#include "Connection.hpp"

namespace web
{
	using boost::asio::io_service;
	using boost::asio::ip::tcp;

	Server::Server(boost::asio::io_service& io_service, const std::string& address, uint32_t port, const std::string& root_dir)
		: io_service{ io_service }, tcp_acceptor{ io_service }, tcp_socket{ io_service }, request_handler{ root_dir }, connection_manager{}
	{
		LOG(INFO) << "Initializing Server.";
		tcp::resolver resolver{ io_service };
		tcp::endpoint endpoint = *resolver.resolve({ address, std::to_string(port) });
		tcp_acceptor.open(endpoint.protocol());
		tcp_acceptor.set_option(tcp::acceptor::reuse_address{ true });
		tcp_acceptor.bind(endpoint);
		tcp_acceptor.listen();
		LOG(INFO) << "Initialized Server.";
	}

	void Server::start()
	{
		LOG(INFO) << "Starting Server.";	
		accept();
		io_service.run();
	}

	void Server::accept()
	{
		LOG(INFO) << "Starting listening for connections...";
		tcp_acceptor.async_accept(tcp_socket, [this](const boost::system::error_code& ec) 
		{
			LOG(INFO) << "Listening for connections...";
			if (!tcp_acceptor.is_open())
			{
				LOG(ERROR) << "tcp_acceptor is not open. Could not accept connection.";
				return;
			}

			if (!ec)
			{
				LOG(INFO) << "New connection accepted.";
				connection_manager.start_connection(std::make_shared<Connection>(
					std::move(tcp_socket), connection_manager, request_handler));
			}

			accept();
		});
	}
}