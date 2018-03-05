#pragma once

#include "ServerFactoryMock.hpp"
#include "ServerMock.hpp"
#include "ServerService.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace web_test
{
    using namespace testing;

    class ServerServiceTestSuite : public Test
    {
    public:
        ServerServiceTestSuite()
            : server_factory{ std::make_unique<ServerFactoryMock>() }
        {
            auto server_mock = std::make_unique<ServerMock>();
            EXPECT_CALL(*server_mock, start());
            EXPECT_CALL(*server_factory, create()).WillOnce(Return(ByMove(std::move(server_mock))));
        }

    protected:
        std::unique_ptr<ServerFactoryMock> server_factory;
    };


    TEST_F(ServerServiceTestSuite, should_start_server_on_service_start)
    {
        using web::ServerService;
        ServerService sut {std::move(server_factory)};
        sut.start();
    }
}