#pragma once

#include "ServerMock.hpp"
#include "IServer.hpp"
#include "IServerFactory.hpp"

#include <memory>

namespace web_test
{
    using web::IServer;
    using web::IServerFactory;

    class ServerFactoryMock : public IServerFactory
    {
    public:
        MOCK_CONST_METHOD0(create, std::unique_ptr<IServer>());
    };
}
