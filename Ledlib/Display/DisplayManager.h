#pragma once

#include <vector>
#include <stdint.h>

namespace Ledlib {

class DisplayManager {
private:
	static int _width, _height;
	static int initCounter;
	DisplayManager();
public:
	static const int& width;
	static const int& height;
	static bool Init();
	static void Sync();
	static int GetBrightness();
	static void SetBrightness(int percent);
	static void Shutdown();
	static void SetPixelsPointer(void* pointer, int pitch);
};

}
