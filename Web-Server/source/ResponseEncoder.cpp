#include "ResponseEncoder.hpp"
#include "Defs.hpp"
#include "Response.hpp"

namespace web
{
	boost::asio::const_buffer ResponseEncoder::to_buffer(ResponseStatus response_status)
	{
		switch (response_status)
		{
		case ResponseStatus::ok:
			return boost::asio::buffer(StatusString::ok);
		case ResponseStatus::created:
			return boost::asio::buffer(StatusString::created);
		case ResponseStatus::accepted:
			return boost::asio::buffer(StatusString::accepted);
		case ResponseStatus::no_content:
			return boost::asio::buffer(StatusString::no_content);
		case ResponseStatus::multiple_choices:
			return boost::asio::buffer(StatusString::multiple_choices);
		case ResponseStatus::moved_permanently:
			return boost::asio::buffer(StatusString::moved_permanently);
		case ResponseStatus::moved_temporarily:
			return boost::asio::buffer(StatusString::moved_temporarily);
		case ResponseStatus::not_modified:
			return boost::asio::buffer(StatusString::not_modified);
		case ResponseStatus::bad_request:
			return boost::asio::buffer(StatusString::bad_request);
		case ResponseStatus::unauthorized:
			return boost::asio::buffer(StatusString::unauthorized);
		case ResponseStatus::forbidden:
			return boost::asio::buffer(StatusString::forbidden);
		case ResponseStatus::not_found:
			return boost::asio::buffer(StatusString::not_found);
		case ResponseStatus::internal_server_error:
			return boost::asio::buffer(StatusString::internal_server_error);
		case ResponseStatus::not_implemented:
			return boost::asio::buffer(StatusString::not_implemented);
		case ResponseStatus::bad_gateway:
			return boost::asio::buffer(StatusString::bad_gateway);
		case ResponseStatus::service_unavailable:
			return boost::asio::buffer(StatusString::service_unavailable);
		default:
			return boost::asio::buffer(StatusString::internal_server_error);
		}
	}

	std::vector<boost::asio::const_buffer> ResponseEncoder::to_buffers(const std::unique_ptr<Response>& response)
	{
		std::vector<boost::asio::const_buffer> buffers;
		buffers.push_back(to_buffer(response->status));
		for (const auto& header : response->headers)
		{
			buffers.push_back(boost::asio::buffer(header.name));
			buffers.push_back(boost::asio::buffer(name_value_separator));
			buffers.push_back(boost::asio::buffer(header.value));
			buffers.push_back(boost::asio::buffer(crlf));
		}
		buffers.push_back(boost::asio::buffer(crlf));
		buffers.push_back(boost::asio::buffer(response->content));
		return buffers;
	}
}