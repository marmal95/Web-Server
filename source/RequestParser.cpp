#include "RequestParser.hpp"
#include "Request.hpp"

#include <cctype>
#include <sstream>
#include <iterator>
#include <boost/algorithm/string.hpp>

namespace web
{
	std::unique_ptr<Request> RequestParser::parse(const boost::asio::const_buffer& buffer)
	{
		Logger::S_LOG << "Parsing new request." << std::endl;
		
		// [TODO]: Throw exception on error
			
		std::string req_content{ boost::asio::buffer_cast<const char*>(buffer) };
		std::vector<std::string> request_lines{};
		boost::split(request_lines, req_content, [](auto c) {return c == '\n'; });

		std::vector<std::string> first_line{};
		boost::split(first_line, request_lines[0], [](auto c) {return c == ' '; });

		auto request = std::make_unique<Request>();
		request->method = first_line[0];
		request->uri = first_line[1];
		request->http_ver = first_line[2];

		return request;
	}
}