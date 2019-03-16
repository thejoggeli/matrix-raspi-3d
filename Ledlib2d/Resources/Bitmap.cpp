#include "Bitmap.h"
#include "../Util/Lodepng.h"
#include "Ledlib/Log.h"
#include "Ledlib/Util/ColorRgb.h"
#include <GLES2/gl2.h>
#include "../Gfx/Renderer.h"
#include "../Gfx/OpenGL.h"

namespace Ledlib {

Bitmap::Bitmap(){}

Bitmap::~Bitmap(){
	glDeleteTextures(1, &textureId);
}

std::shared_ptr<Bitmap> Bitmap::CreateEmpty(uint32_t width, uint32_t height){
	std::shared_ptr<Bitmap> bitmap = std::make_shared<Bitmap>();
	bitmap->_width = width;
	bitmap->_height = height;
	bitmap->_numPixels = width*height;
	bitmap->image.assign(bitmap->_numPixels*4, 0);
	glGenTextures(1, &bitmap->textureId);
	glBindTexture(GL_TEXTURE_2D, bitmap->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return bitmap;
}

std::shared_ptr<Bitmap> Bitmap::CreateRenderable(uint32_t width, uint32_t height){
	std::shared_ptr<Bitmap> bitmap = std::make_shared<Bitmap>();
	bitmap->_width = width;
	bitmap->_height = height;
	bitmap->_numPixels = width*height;
	bool ret = Gfx::GenerateTextureFromFramebuffer(&bitmap->textureId, &bitmap->framebufferId, width, height);
	if(!ret){
		return nullptr;
	}
	glBindTexture(GL_TEXTURE_2D, bitmap->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->width, bitmap->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return bitmap;
}

std::shared_ptr<Bitmap> Bitmap::CreateFromPng(const std::string& path){
	std::shared_ptr<Bitmap> bitmap = std::make_shared<Bitmap>();
	uint32_t error = lodepng::decode(bitmap->image, bitmap->_width, bitmap->_height, path);
	if(error){
		Log(LOG_ERROR, "Error", iLog << "couldn't load png: " << error << lodepng_error_text(error));
		return nullptr;
	}
	bitmap->_numPixels = bitmap->_width * bitmap->_height;
	glGenTextures(1, &bitmap->textureId);
	glBindTexture(GL_TEXTURE_2D, bitmap->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->width, bitmap->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return bitmap;
}

void Bitmap::Update(){
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
}

void Bitmap::Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	for(uint32_t i = 0; i < image.size(); i+=4){
		image[i] = r;
		image[i+1] = g;
		image[i+2] = b;
		image[i+3] = a;
	}
}

void Bitmap::SetPixel(int x, int y, const ColorRgb& color){
	float off = (y*width+x)*4;
	image[off] = static_cast<uint8_t>(color.r >= 1.0f ? 255 : color.r*255.0f);
	image[off+1] = static_cast<uint8_t>(color.g >= 1.0f ? 255 : color.g*255.0f);
	image[off+2] = static_cast<uint8_t>(color.b >= 1.0f ? 255 : color.b*255.0f);
	image[off+3] = static_cast<uint8_t>(color.a >= 1.0f ? 255 : color.a*255.0f);
}

void Bitmap::SetRenderTarget(bool b){
	if(b){
		Gfx::SetRenderTarget(this);
	} else {
		Gfx::SetRenderTarget(nullptr);
	}
}

}
