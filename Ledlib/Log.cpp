#include "Log.h"
#include "Config.h"

#include <iostream>

using namespace std;

namespace Ledlib {

LogConcator LogConcator::instance = LogConcator();

int LogLevels::initCounter = 0;
bool LogLevels::error = true;
bool LogLevels::debug = true;
bool LogLevels::info = true;

bool LogLevels::Init(){
	if(++initCounter > 1) return false;
	info = Config::GetBool("log_info", true);
	debug = Config::GetBool("log_debug", true);
	error = Config::GetBool("log_error", true);
	return true;
}

void Log(LogLevel level, const std::string& source, LogConcator& log){
	Log(level, source, log.stringStream.str());
	log.stringStream.str(std::string());
}
void Log(LogLevel level, const std::string& source, const std::string& str){
	switch(level){
	case LOG_INFO:
		if(LogLevels::info) cout << "[I/" << source << "] " << str << endl;
		break;
	case LOG_DEBUG:
		if(LogLevels::debug) cout << "[D/" << source << "] " << str << endl;
		break;
	case LOG_ERROR:
		if(LogLevels::error) cout << "[E/" << source << "] " << str << endl;
		break;
	}
}

}
