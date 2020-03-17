#include "FileHelper.h"
#include "Ledlib/Log.h"
#include <sys/stat.h>

namespace Ledlib {
namespace FileHelper {

bool CreateDirectory(const std::string& path){
	Log(LOG_INFO, "FileHelper", "mkdir " + path);
	int ret = mkdir(path.c_str(),  ACCESSPERMS);
	if(ret == 0){
		return true;
	}
	switch(errno){
	case EEXIST:
		return true;
	case EACCES:
		Log(LOG_INFO, "FileHelper", "mkdir error code: " + std::to_string(errno) + " (EACCES)");
		break;
	case ELOOP:
		Log(LOG_INFO, "FileHelper", "mkdir error code: " + std::to_string(errno) + " (ELOOP)");
		break;
	case ENOTDIR:
		Log(LOG_INFO, "FileHelper", "mkdir error code: " + std::to_string(errno) + " (ENOTDIR)");
		break;
	case ENOENT:
		Log(LOG_INFO, "FileHelper", "mkdir error code: " + std::to_string(errno) + " (ENOENT)");
		break;
	case ENAMETOOLONG:
		Log(LOG_INFO, "FileHelper", "mkdir error code: " + std::to_string(errno) + " (ENAMETOOLONG)");
		break;
	case ENOSPC:
		Log(LOG_INFO, "FileHelper", "mkdir error code: " + std::to_string(errno) + " (ENOSPC)");
		break;
	case EMLINK:
		Log(LOG_INFO, "FileHelper", "mkdir error code: " + std::to_string(errno) + " (EMLINK)");
		break;
	case EROFS:
		Log(LOG_INFO, "FileHelper", "mkdir error code: " + std::to_string(errno) + " (EROFS)");
		break;
	default:
		Log(LOG_INFO, "FileHelper", "mkdir error code: " + std::to_string(errno) + " (?)");
		break;
	}
	return false;
}

}
}
