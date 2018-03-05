#include "ServerFactory.hpp"
#include "Server.hpp"

namespace web
{
    ServerFactory::ServerFactory(std::string_view root_dir, std::string_view address, uint32_t port)
        : root_dir{ root_dir }, address{ address }, port{ port }
    {}

    std::unique_ptr<IServer> ServerFactory::create() const
    {
        return std::make_unique<Server>(address, port, root_dir);
    }
}
