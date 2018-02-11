#pragma once

#include <iostream>
#include <sstream>
#include <ostream>
#include <chrono>
#include <iomanip>

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

	std::string return_current_time_and_date();
	std::string return_info_level_str();
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
		return_current_time_and_date() <<
		return_info_level_str() <<
		o_stream.str() << os;
	o_stream.str("");
	info_level = InfoLevel::INF;
	return *this;
}

struct Logger
{
	static ServerLog S_LOG;
};