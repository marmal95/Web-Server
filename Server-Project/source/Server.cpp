#include "Server.hpp"
#include "Logger.hpp"

namespace web
{
	Server::Server(const std::shared_ptr<boost::asio::io_service>& io_service)
		: io_service{ io_service }, tcp_endpoint{ tcp::v4(), 2000 }, tcp_acceptor{ *io_service, tcp_endpoint }, tcp_socket{ *io_service }
	{
		tcp_acceptor.listen();
		accept();
	}

	void Server::start()
	{
		io_service->run();
	}

	void Server::accept()
	{
		tcp_acceptor.async_accept(tcp_socket, [this](const boost::system::error_code& ec) 
		{
			LOG(INFO) << "Accepted!";
			accept();
		});
	}
}