#include "Log.h"
#include "Config.h"

#include <iostream>
#include <time.h>

using namespace std;

namespace Ledlib {

static char timeBuffer[32];
static char prefixBuffer[64];
static char prefixBufferPadding[] = ".............";

static const int max_str_len = 150;
static const int max_str_len_cut = max_str_len-3;

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

void Log(LogConcator& log){
	Log(LOG_DEBUG, "Debug", log);
}
void Log(const std::string& str){
	Log(LOG_DEBUG, "Debug", str);
}

void Log(LogLevel level, const std::string& source, LogConcator& log){
	Log(level, source, log.stringStream.str());
	log.stringStream.str(std::string());
}
void Log(LogLevel level, const std::string& source, const std::string& str){
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime (&rawtime);
	strftime(timeBuffer,sizeof(timeBuffer),"%H:%M:%S",timeinfo);
	int padLen = sizeof(prefixBufferPadding) - source.length();
	if(padLen < 0) padLen = 0;
	sprintf(prefixBuffer, "[%s][%s%*.*s]", timeBuffer, source.c_str(), padLen, padLen, prefixBufferPadding);
	if(str.length() > max_str_len){
		switch(level){
		case LOG_INFO:
			if(LogLevels::info) cout << prefixBuffer << " INFO: " << str.substr(0, max_str_len_cut) << "..." << endl;
			break;
		case LOG_DEBUG:
			if(LogLevels::info) cout << prefixBuffer << " INFO: " << str.substr(0, max_str_len_cut) << "..." << endl;
			break;
		case LOG_ERROR:
			if(LogLevels::info) cout << prefixBuffer << " INFO: " << str.substr(0, max_str_len_cut) << "..." << endl;
			break;
		}
	} else {
		switch(level){
		case LOG_INFO:
			if(LogLevels::info) cout << prefixBuffer << " INFO: " << str << endl;
			break;
		case LOG_DEBUG:
			if(LogLevels::debug) cout << prefixBuffer << " DEBUG: " << str << endl;
			break;
		case LOG_ERROR:
			if(LogLevels::error) cerr << prefixBuffer << " ERROR: " << str << endl;
			break;
		}
	}
}

}
