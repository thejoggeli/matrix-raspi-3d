#pragma once

#include <string>
#include <map>

namespace Ledlib {

class Bitmap;
class BaseFont;

namespace ResourceManager {

extern std::string resourcesPath;

bool Init();

// bitmaps
Bitmap* LoadBitmapPng(const std::string& name, const std::string& path);
Bitmap* GetBitmap(const std::string& name);

// fonts
BaseFont* LoadFontBdf(const std::string& name, const std::string& path);
BaseFont* GetFont(const std::string& name);

}

}
