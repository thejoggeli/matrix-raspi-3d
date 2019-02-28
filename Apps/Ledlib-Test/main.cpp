#include <iostream>

#include "Ledlib/Ledlib.h"
#include "Ledlib/Log.h"
#include "Ledlib/Display/DisplayManager.h"
#include "Ledlib/Time.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Sfx/RemoteSfx.h"

using namespace std;
using namespace Ledlib;

int main(){
	LedMatrixLibrary::Init();
	LedMatrixLibrary::Start();

	std::vector<uint8_t> pixels;
	pixels.assign(DisplayManager::width*DisplayManager::height*3, 0);
	DisplayManager::SetPixelsPointer(&pixels, 3);

	RemoteSfx::AddFile("tetris", "tetris/a-theme.mp3");
	RemoteSfx::StartMusic(0, "tetris");

	while(!LedMatrixLibrary::exitRequested){
		LedMatrixLibrary::Update();
		pixels[0] = (pixels[0]+1)%256u;
		pixels[4] = (pixels[0]+1)%256u;
		pixels[8] = (pixels[0]+1)%256u;
		LedMatrixLibrary::Render();
	}
	LedMatrixLibrary::Exit();

	return EXIT_SUCCESS;
}
