#pragma once

#include <memory>

namespace web
{
    class IServer;

    class IServerFactory
    {
    public:
        virtual std::unique_ptr<IServer> create() const = 0;
        virtual ~IServerFactory() = default;
    };
}
