#pragma once

#include "IServerFactory.hpp"

namespace web
{
    class ServerFactory : public IServerFactory
    {
    public:
        ServerFactory(std::string_view root_dir, std::string_view address, uint32_t port);
        std::unique_ptr<IServer> create() const override;

    private:
        std::string root_dir;
        std::string address;
        uint32_t port;
    };
}
