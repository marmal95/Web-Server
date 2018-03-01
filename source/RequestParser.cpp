#include "RequestParser.hpp"
#include "Request.hpp"

#include <cctype>

namespace web
{

RequestParser::RequestParser()
	: request{ nullptr }, state(ParserState::method_start)
{}

std::pair<ResultType, Request_uPtr> RequestParser::parse(const boost::asio::const_buffer& buffer)
{
	Logger::S_LOG << "Parsing new request." << std::endl;
	request = std::make_unique<Request>();

	auto begin = boost::asio::buffer_cast<const char*>(buffer);
	auto end = begin + boost::asio::buffer_size(buffer);

	while (begin != end)
	{
		ResultType result = consume(*begin++);

		if (result == ResultType::good || result == ResultType::bad)
		{
			return { std::move(result), std::move(request) };
		}
	}

	return { ResultType::indeterminate, std::move(request) };
}

void RequestParser::reset()
{
    state = ParserState::method_start;
    request.release();
}

ResultType RequestParser::consume(char input)
{
    switch (state)
    {
        case ParserState::method_start:
            if (!std::isgraph(input) || std::iscntrl(input) || is_special(input))
            {
                return ResultType::bad;
            }
            else
            {
                state = ParserState::method;
                request->method.push_back(input);
                return ResultType::indeterminate;
            }
        case ParserState::method:
            if (input == ' ')
            {
                state = ParserState::uri;
                return ResultType::indeterminate;
            }
            else if (!std::isgraph(input) || std::iscntrl(input) || is_special(input))
            {
                return ResultType::bad;
            }
            else
            {
                request->method.push_back(input);
                return ResultType::indeterminate;
            }
        case ParserState::uri:
            if (input == ' ')
            {
                state = ParserState::http_version_h;
                return ResultType::indeterminate;
            }
            else if (std::iscntrl(input))
            {
                return ResultType::bad;
            }
            else
            {
                request->uri.push_back(input);
                return ResultType::indeterminate;
            }
        case ParserState::http_version_h:
            if (input == 'H')
            {
                state = ParserState::http_version_t_1;
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::http_version_t_1:
            if (input == 'T')
            {
                state = ParserState::http_version_t_2;
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::http_version_t_2:
            if (input == 'T')
            {
                state = ParserState::http_version_p;
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::http_version_p:
            if (input == 'P')
            {
                state = ParserState::http_version_slash;
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::http_version_slash:
            if (input == '/')
            {
                request->http_version_major = 0;
                request->http_version_minor = 0;
                state = ParserState::http_version_major_start;
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::http_version_major_start:
            if (std::isdigit(input))
            {
                request->http_version_major = request->http_version_major * 10 + input - '0';
                state = ParserState::http_version_major;
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::http_version_major:
            if (input == '.')
            {
                state = ParserState::http_version_minor_start;
                return ResultType::indeterminate;
            }
            else if (std::isdigit(input))
            {
                request->http_version_major = request->http_version_major * 10 + input - '0';
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::http_version_minor_start:
            if (std::isdigit(input))
            {
                request->http_version_minor = request->http_version_minor * 10 + input - '0';
                state = ParserState::http_version_minor;
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::http_version_minor:
            if (input == '\r')
            {
                state = ParserState::expecting_newline_1;
                return ResultType::indeterminate;
            }
            else if (std::isdigit(input))
            {
                request->http_version_minor = request->http_version_minor * 10 + input - '0';
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::expecting_newline_1:
            if (input == '\n')
            {
                state = ParserState::header_line_start;
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::header_line_start:
            if (input == '\r')
            {
                state = ParserState::expecting_newline_3;
                return ResultType::indeterminate;
            }
            else if (!request->headers.empty() && (input == ' ' || input == '\t'))
            {
                state = ParserState::header_lws;
                return ResultType::indeterminate;
            }
            else if (!std::isgraph(input) || std::iscntrl(input) || is_special(input))
            {
                return ResultType::bad;
            }
            else
            {
                request->headers.emplace_back(Header{});
                request->headers.back().name.push_back(input);
                state = ParserState::header_name;
                return ResultType::indeterminate;
            }
        case ParserState::header_lws:
            if (input == '\r')
            {
                state = ParserState::expecting_newline_2;
                return ResultType::indeterminate;
            }
            else if (input == ' ' || input == '\t')
            {
                return ResultType::indeterminate;
            }
            else if (std::iscntrl(input))
            {
                return ResultType::bad;
            }
            else
            {
                state = ParserState::header_value;
                request->headers.back().value.push_back(input);
                return ResultType::indeterminate;
            }
        case ParserState::header_name:
            if (input == ':')
            {
                state = ParserState::space_before_header_value;
                return ResultType::indeterminate;
            }
            else if (!std::isgraph(input) || std::iscntrl(input) || is_special(input))
            {
                return ResultType::bad;
            }
            else
            {
                request->headers.back().name.push_back(input);
                return ResultType::indeterminate;
            }
        case ParserState::space_before_header_value:
            if (input == ' ')
            {
                state = ParserState::header_value;
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::header_value:
            if (input == '\r')
            {
                state = ParserState::expecting_newline_2;
                return ResultType::indeterminate;
            }
            else if (std::iscntrl(input))
            {
                return ResultType::bad;
            }
            else
            {
                request->headers.back().value.push_back(input);
                return ResultType::indeterminate;
            }
        case ParserState::expecting_newline_2:
            if (input == '\n')
            {
                state = ParserState::header_line_start;
                return ResultType::indeterminate;
            }
            else
            {
                return ResultType::bad;
            }
        case ParserState::expecting_newline_3:
            return (input == '\n') ? ResultType ::good : ResultType::bad;
		default:
			return ResultType::bad;
    }
}

bool RequestParser::is_special(int c) const
{
    switch (c)
    {
        case '(': case ')': case '<': case '>': case '@':
        case ',': case ';': case ':': case '\\': case '"':
        case '/': case '[': case ']': case '?': case '=':
        case '{': case '}': case ' ': case '\t':
            return true;
        default:
            return false;
    }
}
}