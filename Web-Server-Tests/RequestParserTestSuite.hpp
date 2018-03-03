#pragma once

#include "RequestParser.hpp"
#include "Request.hpp"
#include "Exception.hpp"

#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/asio/buffer.hpp>

namespace web_test
{
	using web::RequestParser;
	using namespace testing;

	const std::string GOOD_REQUEST {	
		"GET /root/testable/directory/ HTTP/1.1\n\r" 
		"User - Agent: Mozilla / 4.0 (compatible; MSIE5.01; Windows NT)\n\r" 
		"Host : www.testable-url.test\n\r" 
		"Content - Type : application / x - www - form - urlencoded\n\r" 
		"Content - Length : length\n\r" 
		"Accept - Language : en - us\n\r" 
		"Accept - Encoding : gzip, deflate\n\r" 
		"Connection : Keep - Alive\n\r"
	};

	const std::string BAD_EMPTY_REQUEST {};

	const std::string BAD_REQUEST_NO_METHOD{
		"/root/testable/directory/ HTTP/1.1\n\r"
		"User - Agent: Mozilla / 4.0 (compatible; MSIE5.01; Windows NT)\n\r"
		"Host : www.testable-url.test\n\r"
		"Content - Type : application / x - www - form - urlencoded\n\r"
		"Content - Length : length\n\r"
	};

	const std::string BAD_REQUEST_NO_URL {
		"GET HTTP/1.1\n\r"
		"User - Agent: Mozilla / 4.0 (compatible; MSIE5.01; Windows NT)\n\r"
		"Host : www.testable-url.test\n\r"
		"Content - Type : application / x - www - form - urlencoded\n\r"
		"Content - Length : length\n\r"
	};

	const std::string BAD_REQUEST_NO_HTTP_VER {
		"GET /root/testable/directory/\n\r"
		"User - Agent: Mozilla / 4.0 (compatible; MSIE5.01; Windows NT)\n\r"
		"Host : www.testable-url.test\n\r"
		"Content - Type : application / x - www - form - urlencoded\n\r"
		"Content - Length : length\n\r"
	};

	const size_t EXPECTED_HEADERS_SIZE {};
	const std::string EXPECTED_METHOD_TYPE{ "GET" };
	const std::string EXPECTED_URL{ "/root/testable/directory/" };
	const std::string EXPECTED_HTTP_VER{ "HTTP/1.1" };

	class RequestParserTestSuite : public testing::Test
	{
	public:
		RequestParserTestSuite()
			: req_parser{}
		{}

	protected:
		RequestParser req_parser;
	};

	TEST_F(RequestParserTestSuite, should_parse_request_when_request_good)
	{
		auto request = req_parser.parse(boost::asio::buffer(GOOD_REQUEST));
		EXPECT_EQ(EXPECTED_METHOD_TYPE, request->method);
		EXPECT_EQ(EXPECTED_URL, request->uri);
		EXPECT_EQ(EXPECTED_HTTP_VER, request->http_ver);
		EXPECT_THAT(request->headers, SizeIs(EXPECTED_HEADERS_SIZE));
	}

	TEST_F(RequestParserTestSuite, should_throw_exception_on_empty_request)
	{
		EXPECT_THROW(req_parser.parse(boost::asio::buffer(BAD_EMPTY_REQUEST)), RequestInvalid);
	}

	TEST_F(RequestParserTestSuite, should_throw_exception_on_bad_request_with_no_method_in_request_line)
	{
		EXPECT_THROW(req_parser.parse(boost::asio::buffer(BAD_REQUEST_NO_METHOD)), RequestInvalid);
	}

	TEST_F(RequestParserTestSuite, should_throw_exception_on_bad_request_with_no_url_in_request_line)
	{
		EXPECT_THROW(req_parser.parse(boost::asio::buffer(BAD_REQUEST_NO_URL)), RequestInvalid);
	}

	TEST_F(RequestParserTestSuite, should_throw_exception_on_bad_request_with_no_http_ver_in_request_line)
	{
		EXPECT_THROW(req_parser.parse(boost::asio::buffer(BAD_REQUEST_NO_HTTP_VER)), RequestInvalid);
	}
}