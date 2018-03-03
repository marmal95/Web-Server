#include "RequestParser.hpp"
#include "Request.hpp"
#include "Exception.hpp"

#include <cctype>
#include <sstream>
#include <iterator>
#include <boost/algorithm/string.hpp>

namespace web
{
	std::unique_ptr<Request> RequestParser::parse(const boost::asio::const_buffer& buffer)
	{
		Logger::S_LOG << "Parsing new request." << std::endl;
		
		std::string req_content{ boost::asio::buffer_cast<const char*>(buffer) };
		auto request_lines = split_lines(req_content);
		conditionally_throw_exception([&request_lines]() { return request_lines.empty(); });

		auto first_line = split_request_line(request_lines);
		conditionally_throw_exception([&first_line]() { return first_line.size() < 3; });

		auto request = std::make_unique<Request>();
		request->method = first_line[0];
		request->uri = first_line[1];
		request->http_ver = first_line[2];

		return request;
	}

	std::vector<std::string> RequestParser::split_lines(std::string_view content) const
	{
		std::vector<std::string> lines{};
		boost::split(lines, content, [](auto c) {return c == '\n'; });
		return lines;
	}

	std::vector<std::string> RequestParser::split_request_line(const std::vector<std::string>& lines) const
	{
		std::vector<std::string> first_line{};
		boost::split(first_line, lines[0], [](auto c) {return c == ' '; });
		return first_line;
	}
}
