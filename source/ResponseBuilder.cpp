#include "ResponseBuilder.hpp"
#include "Response.hpp"

namespace web
{
	ResponseBuilder::ResponseBuilder()
		: status_str_responses{}, extensions{}
	{
		fill_status_str_responses();
		fill_extensions_map();
	}

	std::unique_ptr<IResponse> web::ResponseBuilder::build(
		ResponseStatus status, std::optional<std::string_view> content, std::optional<std::string_view> extension) const
	{
		auto resp_content = content ? content->data() : "";
		auto resp_extension = extension ? extension->data() : "html";

		auto response = std::make_unique<Response>();
		response->status = status;
		response->content = status_str_responses.find(status)->second;
		response->content.append(resp_content);
		response->headers.resize(2);
		response->headers[0].name = "Content-Length";
		response->headers[0].value = std::to_string(response->content.size());
		response->headers[1].name = "Content-Type";
		response->headers[1].value = extensions.find(resp_extension)->second;
		return response;
	}

	void ResponseBuilder::fill_status_str_responses()
	{
		status_str_responses[ResponseStatus::ok] = {};
		status_str_responses[ResponseStatus::created] = 
			"<html>"
			"<head><title>Created</title></head>"
			"<body><h1>201 Created</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::accepted] =
			"<html>"
			"<head><title>Accepted</title></head>"
			"<body><h1>202 Accepted</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::no_content] =
			"<html>"
			"<head><title>No Content</title></head>"
			"<body><h1>204 Content</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::multiple_choices] =
			"<html>"
			"<head><title>Multiple Choices</title></head>"
			"<body><h1>300 Multiple Choices</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::moved_permanently] =
			"<html>"
			"<head><title>Moved Permanently</title></head>"
			"<body><h1>301 Moved Permanently</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::moved_temporarily] =
			"<html>"
			"<head><title>Moved Temporarily</title></head>"
			"<body><h1>302 Moved Temporarily</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::not_modified] = 		
			"<html>"
			"<head><title>Not Modified</title></head>"
			"<body><h1>304 Not Modified</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::bad_request] =
			"<html>"
			"<head><title>Bad Request</title></head>"
			"<body><h1>400 Bad Request</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::unauthorized] =
			"<html>"
			"<head><title>Unauthorized</title></head>"
			"<body><h1>401 Unauthorized</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::forbidden] =
			"<html>"
			"<head><title>Forbidden</title></head>"
			"<body><h1>403 Forbidden</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::not_found] =
			"<html>"
			"<head><title>Not Found</title></head>"
			"<body><h1>404 Not Found</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::internal_server_error] =
			"<html>"
			"<head><title>Internal Server Error</title></head>"
			"<body><h1>500 Internal Server Error</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::not_implemented] =
			"<html>"
			"<head><title>Not Implemented</title></head>"
			"<body><h1>501 Not Implemented</h1></body>"
			"</html>";;
		status_str_responses[ResponseStatus::bad_gateway] =
			"<html>"
			"<head><title>Bad Gateway</title></head>"
			"<body><h1>502 Bad Gateway</h1></body>"
			"</html>";
		status_str_responses[ResponseStatus::service_unavailable] = 
			"<html>"
			"<head><title>Service Unavailable</title></head>"
			"<body><h1>503 Service Unavailable</h1></body>"
			"</html>";
	}

	void ResponseBuilder::fill_extensions_map()
	{
		extensions["gif"] = "image/gif";
		extensions["htm"] = "text/html";
		extensions["html"] = "text/html";
		extensions["jpg"] = "image/jpeg";
		extensions["png"] = "image/png";
	}

}