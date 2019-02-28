#pragma once

#include <string>

namespace Ledlib {

class LedMatrixLibrary {
private:
	static int initCounter;
	static int startCounter;
	static int exitCounter;
public:
	static bool exitRequested;
	static bool Init();
	static void Start();
	static void Update();
	static void Render();
	static void Exit();
	static void RequestExit();
};

}

