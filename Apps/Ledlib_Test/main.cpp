#include <iostream>

#include "Ledlib/Ledlib.h"
#include "Ledlib/Log.h"
#include "Ledlib/Display/DisplayManager.h"

using namespace std;
using namespace Ledlib;

int main(){
	LedMatrixLibrary::Init();
	LedMatrixLibrary::Start();

	std::vector<uint8_t> pixels;
	pixels.assign(DisplayManager::width*DisplayManager::height*3, 0);
	DisplayManager::SetPixelsPointer(&pixels, 3);

	while(!LedMatrixLibrary::exitRequested){
		pixels[0] = (pixels[0]+1)%256u;
		LedMatrixLibrary::Update();
	}
	LedMatrixLibrary::Exit();

	return EXIT_SUCCESS;
}
