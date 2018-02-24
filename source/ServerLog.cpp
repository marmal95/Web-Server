#include "ServerLog.hpp"
#include <boost/thread.hpp>

ServerLog Logger::S_LOG{};


std::string ServerLog::return_curr_time_and_date() const
{
	std::ostringstream os{};
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	os << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
	os << "." << std::setfill('0') << std::setw(3) << ms.count();
	return os.str();
}

std::string ServerLog::return_info_level_str() const
{
	switch (info_level)
	{
	case InfoLevel::INF: return " [INFO] ";
	case InfoLevel::DBG: return " [DEBUG] ";
	case InfoLevel::WRN: return " [WARNING] ";
	case InfoLevel::ERR: return " [ERROR] ";
	}
	return {};
}

std::string ServerLog::return_curr_thread_id() const
{
	std::ostringstream os{};
	os << "[";
	os << boost::this_thread::get_id();
	os << "]";
	return os.str();
}

std::string ServerLog::return_arrow_pointer() const
{
	return " => ";
}
