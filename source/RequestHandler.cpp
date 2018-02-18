#include "RequestHandler.hpp"
#include "ServerLog.hpp"
#include "Exception.hpp"
#include "ServerLog.hpp"

#include <fstream>

namespace web
{
	RequestHandler::RequestHandler(const ResponseBuilder& response_builder, const std::string& root_dir)
		: response_builder{ response_builder }, root_dir { root_dir}
	{
		Logger::S_LOG << "Initializing RequestHandler..." << std::endl;
		Logger::S_LOG << "Initialized RequestHandler." << std::endl;
	}

	std::unique_ptr<IResponse> RequestHandler::handle_request(std::unique_ptr<Request> request)
	{
		Logger::S_LOG << "Received new Request to handle." << std::endl;
		Logger::S_LOG << "Start handling new Request..." << std::endl;

		std::string request_path{ request->uri };
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
			Logger::S_LOG << "Creating response for: " << full_path << std::endl;
			return response_builder.build(ResponseStatus::ok, content, extension);
		} 
		catch (const Exception& exc)
		{
			Logger::S_LOG << exc.what() << std::endl;
			return response_builder.build(ResponseStatus::not_found, "");
		}
	}

	bool RequestHandler::is_path_correct(const std::string& path) const
	{
		return !path.empty() && path[0] == '/' && path.find("..") == std::string::npos;
	}

	bool RequestHandler::is_path_dir(const std::string& path) const
	{
		return path[path.size() - 1] == '/';
	}

	void RequestHandler::append_index(std::string& path)
	{
		path += "index.html";
	}

	std::string RequestHandler::get_extension(const std::string & path) const
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

	std::string RequestHandler::read_file_content(const std::string& path)
	{
		std::string content{};

		std::ifstream input_file(path, std::ios::binary);
		input_file.unsetf(std::ios::skipws);
		if (!input_file)
		{
			throw Exception{ "File " + path + " not found!" };
		}

		content.insert(content.begin(),
			std::istream_iterator<char>(input_file),
			std::istream_iterator<char>());

		input_file.close();
		return content;
	}
}
