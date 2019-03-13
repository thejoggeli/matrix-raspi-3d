#include <iostream>

#include "ShaderPlayground.h"

using namespace Ledlib;

int main(int argc, char** argv){
	ShaderPlayground* game = new ShaderPlayground();
	game->file = argc > 1 ? argv[1] : "mandelbrot.frag";
	game->file = "Shaders/" + game->file;
	ShaderPlayground::Run(new ShaderPlayground());
	return EXIT_SUCCESS;
}
