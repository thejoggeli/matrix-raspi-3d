#include <iostream>
#include "Game2d.h"
#include "Ledlib/Config.h"

int main(){
//	Config::Set("opengl_multisampling_enabled", "1", Config::Target::Runtime);
//	Config::Set("opengl_multisampling_samples", "2", Config::Target::Runtime);
	Game2d::Run(new Game2d());
	return EXIT_SUCCESS;
}
