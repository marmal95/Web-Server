#include "Server.hpp"
#include "Connection.hpp"
#include "ServerLog.hpp"

namespace web
{
	using boost::asio::io_service;
	using boost::asio::ip::tcp;

	Server::Server(boost::asio::io_service& io_service, const std::string& address, uint32_t port, const std::string& root_dir)
		: io_service{ io_service }, tcp_acceptor{ io_service }, tcp_socket{ io_service }, request_handler{ root_dir }, connection_manager{}
	{
		Logger::S_LOG << "Initializing Server..." << std::endl;
		tcp::resolver resolver{ io_service };
		tcp::endpoint endpoint = *resolver.resolve({ address, std::to_string(port) });
		tcp_acceptor.open(endpoint.protocol());
		tcp_acceptor.set_option(tcp::acceptor::reuse_address{ true });
		tcp_acceptor.bind(endpoint);
		tcp_acceptor.listen();
		Logger::S_LOG << "Initialized Server." << std::endl;
	}

	void Server::start()
	{
		Logger::S_LOG << "Starting Server..." << std::endl;
		accept();
		io_service.run();
	}

	void Server::accept()
	{
		Logger::S_LOG << "Starting listening for connections..." << std::endl;
		tcp_acceptor.async_accept(tcp_socket, [this](const boost::system::error_code& ec) 
		{
			Logger::S_LOG << "Listening for connections..." << std::endl;
			if (!tcp_acceptor.is_open())
			{
				Logger::S_LOG(InfoLevel::ERR) << __FILE__ << "#" << __LINE__ << ":" << __FUNCTION__ << "tcp_acceptor is not open. Could not accept connection." << std::endl;
				return;
			}

			if (!ec)
			{
				Logger::S_LOG << "New Connection accepted: " << "[" << tcp_socket.remote_endpoint().address().to_string() << "]" << std::endl;
				connection_manager.start_connection(std::make_shared<Connection>(
					std::move(tcp_socket), connection_manager, request_handler));
			}

			accept();
		});
	}
}