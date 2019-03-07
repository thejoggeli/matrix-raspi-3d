#include "Gfx.h"
#include "OpenGL.h"
#include "Transform.h"
#include "Renderer.h"
#include "Bitmaps.h"
#include "Draw.h"
#include "Shaders.h"
#include "Ledlib/Display/DisplayManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLES2/gl2.h>
#include <stdlib.h>

namespace Ledlib {

namespace Gfx {

int width = 0, height = 0;
float left = 0, right = 0, bottom = 0, top = 0;

bool Init(){

	width = DisplayManager::width;
	height = DisplayManager::height;

	if(!InitOpenGL(width, height)){
		return false;
	}
	left = -static_cast<float>(width)/2.0f;
	right = static_cast<float>(width)/2.0f;
	top = static_cast<float>(height)/2.0f;
	bottom = -static_cast<float>(height)/2.0f;

	// setup transform
	SetProjectionMatrix(glm::ortho(left, right, bottom, top, -100000.0f, 100000.0f));
	Scale(1, -1, 1);
	Translate(0.0666f, 0.0666f);
	Save();

	// setup colors
	SetClearColor(0, 0, 0, 1);
	SetDrawColor(1, 1, 1, 1);
	SetBitmapColor(1, 1, 1, 1);

	InitRenderer(width, height);
	InitDraw(width, height);
	InitShaders();

	return true;
}

}
}
