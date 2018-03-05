#pragma once

#include "IServer.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace web_test
{
    using web::IServer;

    class ServerMock : public IServer
    {
    public:
        MOCK_METHOD0(start, void());
        MOCK_METHOD0(stop, void());
        MOCK_METHOD0(reset, void());
    };
}
