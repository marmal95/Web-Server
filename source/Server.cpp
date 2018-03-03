#include "Server.hpp"
#include "Connection.hpp"
#include "ServerLog.hpp"


namespace web
{
	using boost::asio::io_service;
	using boost::asio::ip::tcp;

	Server::Server(std::string_view address, uint32_t port, std::string_view root_dir)
		: service_io{}, tcp_acceptor{ service_io }, tcp_socket{ service_io },
		response_builder{}, request_handler{ response_builder, root_dir }, request_parser{}, connection_manager {}
	{
		Logger::S_LOG << "Initializing Server..." << std::endl;
		tcp::resolver resolver{ service_io };
		tcp::endpoint endpoint = *resolver.resolve({ address.data(), std::to_string(port) });
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
		service_io.run();
	}

	void Server::stop()
	{
		Logger::S_LOG << "Stopping Server..." << std::endl;
		service_io.stop();
		tcp_acceptor.cancel();
		tcp_acceptor.close();
		tcp_socket.close();
		connection_manager.stop_all_connections();
	}

	void Server::reset()
	{
		stop();
		start();
	}

	void Server::accept()
	{
        Logger::S_LOG << "Listening for connections..." << std::endl;
		tcp_acceptor.async_accept(tcp_socket, [this](const boost::system::error_code& ec)
		{
			if (!tcp_acceptor.is_open())
			{
				Logger::S_LOG(InfoLevel::ERR) << __FILE__ << "#" << __LINE__ << " => " <<
					__FUNCTION__ << "(): tcp_acceptor is not open. Could not accept connection." << std::endl;
				return;
			}

			if (!ec)
			{
				Logger::S_LOG << "New Connection accepted: " << "[" << tcp_socket.remote_endpoint().address().to_string() << "]" << std::endl;
                connection_manager.start_connection(std::make_shared<Connection>(
					std::move(tcp_socket), connection_manager, request_handler, request_parser));
			}

			accept();
		});
	}
}