#pragma once

#include <iostream>
#include <sstream>
#include <ostream>
#include <chrono>
#include <iomanip>
#include <boost/thread.hpp>

enum class InfoLevel
{
	INF,
	DBG,
	WRN,
	ERR
};

class ServerLog
{
public:
	static ServerLog S_LOG;

	ServerLog() : info_level{ InfoLevel::INF }, o_stream{} {}

	ServerLog& operator()(InfoLevel il);
	template<typename T>
	ServerLog& operator<<(T&& value);
	ServerLog& operator<<(std::ostream& (*os)(std::ostream&));

private:
	InfoLevel info_level;
	std::ostringstream o_stream;

	std::string return_curr_time_and_date() const;
	std::string return_info_level_str() const;
	std::string return_curr_thread_id() const;
	std::string return_arrow_pointer() const;
};

inline ServerLog& ServerLog::operator()(InfoLevel il)
{
	info_level = il;
	return *this;
}

template<typename T>
inline ServerLog& ServerLog::operator<<(T&& value)
{
	o_stream << value;
	return *this;
}

inline ServerLog& ServerLog::operator<<(std::ostream& (*os)(std::ostream&))
{
	std::cout <<
		return_curr_time_and_date() <<
		return_info_level_str() <<
		return_curr_thread_id() <<
		return_arrow_pointer() <<
		o_stream.str() << os;
	o_stream.str("");
	info_level = InfoLevel::INF;
	return *this;
}

struct Logger
{
	static ServerLog S_LOG;
};