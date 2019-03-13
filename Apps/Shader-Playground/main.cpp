#include <iostream>

#include "ShaderPlayground.h"
#include "Ledlib/Log.h"

using namespace Ledlib;

int main(int argc, char** argv){
	ShaderPlayground* game = new ShaderPlayground();
	game->file = argc > 1 ? argv[1] : "mandelbrot.frag";
	game->file = "playground/" + game->file;
	Log(LOG_INFO, "ShaderPlayground", game->file);
	ShaderPlayground::Run(game);
	return EXIT_SUCCESS;
}
