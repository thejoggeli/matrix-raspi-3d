#pragma once

#define iLog (Ledlib::LogConcator::instance)

#include <sstream>

enum LogLevel {
	LOG_ERROR = 0, LOG_DEBUG = 10, LOG_INFO = 20
};

namespace Ledlib {

class LogLevels {
private:
	static int initCounter;
public:
	static bool error;
	static bool debug;
	static bool info;
	static bool Init();
};

class LogConcator {
private:
public:
	std::stringstream stringStream;
	static LogConcator instance;
	template<typename T>
	LogConcator& operator << (const T& val){
		stringStream << val;
		return *this;
	}
};

void Log(LogConcator& log);
void Log(const std::string& str);
void Log(LogLevel level, const std::string& source, LogConcator& log);
void Log(LogLevel level, const std::string& source, const std::string& str);

}
