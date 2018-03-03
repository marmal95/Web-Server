#include "RequestHandler.hpp"
#include "ServerLog.hpp"
#include "Exception.hpp"
#include "ServerLog.hpp"

#include <fstream>
#include <iterator>

namespace web
{
	RequestHandler::RequestHandler(const ResponseBuilder& response_builder, std::string_view root_dir)
		: response_builder{ response_builder }, root_dir { root_dir}
	{
		Logger::S_LOG << "Initializing RequestHandler..." << std::endl;
		Logger::S_LOG << "Initialized RequestHandler." << std::endl;
	}

	std::unique_ptr<Response> RequestHandler::handle_request(std::unique_ptr<Request> request)
	{
		Logger::S_LOG << "Received new Request to handle." << std::endl;
		Logger::S_LOG << "Start handling new Request..." << std::endl;

		std::string request_path{ request->uri };
		request_path = decode_path(request_path);
		if (is_path_dir(request_path))
		{
			Logger::S_LOG << "Path to dir in request. Appending index.html" << std::endl;
			append_index(request_path);
		}

		try
		{
			std::string extension = get_extension(request_path);
			std::string full_path = root_dir + request_path;
			std::string content = read_file_content(full_path);
			Logger::S_LOG << "Creating response for: " << request_path << std::endl;
			return response_builder.build(ResponseStatus::ok, content, extension);
		} 
		catch (const FileNotFound& exc)
		{
			Logger::S_LOG << exc.what() << " [ " << request_path << " ] " << std::endl;
			return response_builder.build(ResponseStatus::not_found, std::nullopt, std::nullopt);
		}
	}

	std::unique_ptr<Response> RequestHandler::handle_bad_request()
	{
		Logger::S_LOG << "Received new bad Request to handle." << std::endl;
		Logger::S_LOG << "Start handling new bad Request..." << std::endl;
		return response_builder.build(ResponseStatus::bad_request, std::nullopt, std::nullopt);
	}

	std::string RequestHandler::decode_path(std::string_view path) const
	{
		std::string dec_path{ path };
		auto path_end_idx = path.find('?');
		if (path_end_idx != std::string::npos)
		{
			dec_path = path.substr(0, path_end_idx);
		}
		return dec_path;
	}

	bool RequestHandler::is_path_correct(std::string_view path) const
	{
		return !path.empty() && path[0] == '/' && path.find("..") == std::string::npos;
	}

	bool RequestHandler::is_path_dir(std::string_view path) const
	{
		return path[path.size() - 1] == '/';
	}

	void RequestHandler::append_index(std::string& path)
	{
		path += "index.html";
	}

	std::string RequestHandler::get_extension(std::string_view path) const
	{
		auto last_slash_pos = path.find_last_of("/");
		auto last_dot_pos = path.find_last_of(".");

		std::string extension{};
		if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
		{
			extension = path.substr(last_dot_pos + 1);
		}
		return extension;
	}

	std::string RequestHandler::read_file_content(std::string_view path)
	{
		std::ifstream input_file(path.data(), std::ios::binary);
		if (!input_file)
		{
			throw FileNotFound{ "Could not open the file."};
		}

		input_file.unsetf(std::ios::skipws);
		input_file.seekg(0, std::ios::end);
		auto file_size = input_file.tellg();
		input_file.seekg(0, std::ios::beg);

		std::string content{};
		content.reserve(file_size);

		content.insert(content.begin(),
			std::istream_iterator<char>(input_file),
			std::istream_iterator<char>());

		input_file.close();
		return content;
	}
}
