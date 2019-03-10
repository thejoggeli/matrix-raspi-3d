#include "Bitmap.h"
#include "../Util/Lodepng.h"
#include "Ledlib/Log.h"
#include "Ledlib/Util/ColorRgb.h"
#include <GLES2/gl2.h>

namespace Ledlib {

Bitmap::Bitmap(){}

Bitmap::~Bitmap(){
	glDeleteTextures(1, &textureId);
}

bool Bitmap::CreateEmpty(uint32_t width, uint32_t height){
	if(created){
		(LOG_ERROR, "Bitmap", "Already created");
		return false;
	}
	_width = width;
	_height = height;
	_numPixels = width*height;
	image.assign(_numPixels*4, 0);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	created = true;
	return true;
}

bool Bitmap::CreateFromPng(const std::string& path){
	if(created){
		(LOG_ERROR, "Bitmap", "Already created");
		return false;
	}
	uint32_t error = lodepng::decode(image, _width, _height, path);
	if(error){
		(LOG_ERROR, "Error", iLog << "couldn't load png: " << error << lodepng_error_text(error));
		return false;
	}
	_numPixels = _width * _height;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	created = true;
	return true;
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

}
