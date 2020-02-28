#include "Gfx.h"
#include "OpenGL.h"
#include "Transform.h"
#include "Renderer.h"
#include "Bitmaps.h"
#include "Draw.h"
#include "Shaders.h"
#include "Ledlib/Display/DisplayManager.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Display/DisplayManager.h"
#include "../Resources/Bitmap.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLES2/gl2.h>
#include <stdlib.h>

namespace Ledlib {

namespace Gfx {

int width = 0, height = 0;
float left = 0, right = 0, bottom = 0, top = 0;

static ColorRgb _clearColor;
static uint8_t* pixels;

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
	ClearTransform();
	Save();

	// setup colors
	SetClearColor(0, 0, 0, 1);
	SetDrawColor(1, 1, 1, 1);
	SetBitmapColor(1, 1, 1, 1);

	// display
	int pitch = 3;
	pixels = (uint8_t*) malloc(width * height * sizeof(uint8_t) * pitch);
	DisplayManager::SetPixelsPointer(pixels, pitch);

	InitRenderer(width, height);
	InitDraw(width, height);
	InitShaders();

	return true;
}

// clear
void Clear(){
	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Clear(float r, float g, float b, float a){
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void SetClearColor(float r, float g, float b, float a){
	_clearColor.r = r; _clearColor.g = g; _clearColor.b = b; _clearColor.a = a;
}
void SetClearColor(const ColorRgb& color){
	_clearColor = color;
}
void SetRenderTarget(const Bitmap* bitmap){
	if(bitmap == nullptr){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Gfx::width, Gfx::height);
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, bitmap->framebufferId);
		glViewport(0, 0, bitmap->width, bitmap->height);
	}
}


void UpdatePixelBuffer(){
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}


}
}
