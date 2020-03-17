#include "ResourceManager.h"
#include "Ledlib/Config.h"
#include "Ledlib/Log.h"
#include "Font.h"
#include "Bitmap.h"
#include <stdio.h>
#include <sys/stat.h>

namespace Ledlib {
namespace ResourceManager {

static std::map<std::string, BaseFont*> fontsMap;
static std::map<std::string, std::shared_ptr<Bitmap>> bitmapsMap;

std::string resourcesPath = "";

bool Init(){
	resourcesPath = LEDLIB_RESOURCES_PATH;
	if(resourcesPath.length() > 0 && resourcesPath[resourcesPath.length()-1] == '/'){
		resourcesPath = resourcesPath.substr(0, resourcesPath.length()-1);
	}
	struct stat sb;
	if (stat(resourcesPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)){
		// directory exists
	} else {
		Log(LOG_ERROR, "Resources", "Resources directory not found:");
		Log(LOG_ERROR, "Resources", resourcesPath);
		Log(LOG_ERROR, "Resources", "Set resources_path=/path/to/resources in matlib.user.cfg");
		return false;
	}
	return true;
}

Bitmap* LoadBitmapPng(const std::string& name, const std::string& path){
	Log(LOG_INFO, "Resources", iLog << "Loading bitmap: " << path);
	std::shared_ptr<Bitmap> bitmap = Bitmap::CreateFromPng(resourcesPath + "/bitmaps/" + path);
	if(!bitmap){
		return nullptr;
	}
	bitmapsMap[name] = bitmap;
	return bitmap.get();
}

Bitmap* GetBitmap(const std::string& name){
	return bitmapsMap[name].get();
}

BaseFont* LoadFontBdf(const std::string& name, const std::string& path){
	Log(LOG_INFO, "Resources", iLog << "Loading font: " << path);
	BaseFont* font = new BaseFont();
	if(!font->LoadBdf(resourcesPath + "/fonts/" + path)){
		return nullptr;
	}
	fontsMap[name] = font;
	return font;
}

BaseFont* GetFont(const std::string& name){
	return fontsMap[name];
}

}
}

