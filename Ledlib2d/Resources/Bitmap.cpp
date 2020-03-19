#include "Bitmap.h"
#include "Ledlib/Util/Lodepng.h"
#include "Ledlib/Log.h"
#include "Ledlib/Util/ColorRgb.h"
#include <GLES2/gl2.h>
#include "../Gfx/Gfx.h"
#include "../Gfx/OpenGL.h"

namespace Ledlib {

Bitmap::Bitmap(){}

Bitmap::~Bitmap(){
	delete[] image;
	glDeleteTextures(1, &textureId);
}

std::shared_ptr<Bitmap> Bitmap::CreateEmpty(uint32_t width, uint32_t height){
	std::shared_ptr<Bitmap> bitmap = std::make_shared<Bitmap>();
	bitmap->_width = width;
	bitmap->_height = height;
	bitmap->_numPixels = width*height;
	bitmap->_size = bitmap->_numPixels*4;
	bitmap->image = new uint8_t[bitmap->_size];
	for(int i = 0; i < bitmap->_size; i++){
		bitmap->image[i] = 0;
	}
	glGenTextures(1, &bitmap->textureId);
	glBindTexture(GL_TEXTURE_2D, bitmap->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->image);
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
	bitmap->_size = width*height*4;
	bitmap->image = new uint8_t[bitmap->_size];
	bool ret = Gfx::GenerateTextureFromFramebuffer(&bitmap->textureId, &bitmap->framebufferId, width, height);
	if(!ret){
		return nullptr;
	}
	glBindTexture(GL_TEXTURE_2D, bitmap->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->width, bitmap->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return bitmap;
}

std::shared_ptr<Bitmap> Bitmap::CreateFromPng(const std::string& path){
	std::shared_ptr<Bitmap> bitmap = std::make_shared<Bitmap>();
	std::vector<uint8_t> image_vector;
	uint32_t error = lodepng::decode(image_vector, bitmap->_width, bitmap->_height, path);
	if(error){
		Log(LOG_ERROR, "Error", iLog << "couldn't load png: " << error << lodepng_error_text(error));
		return nullptr;
	}
	bitmap->_numPixels = bitmap->_width * bitmap->_height;
	bitmap->_size = bitmap->_numPixels*4;
	bitmap->image = new uint8_t[bitmap->_size];
	for(int i = 0; i < bitmap->_size; i++){
		bitmap->image[i] = image_vector[i];
	}
	glGenTextures(1, &bitmap->textureId);
	glBindTexture(GL_TEXTURE_2D, bitmap->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->width, bitmap->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return bitmap;
}

void Bitmap::Update(){
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

void Bitmap::Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	for(uint32_t i = 0; i < size; i+=4){
		image[i] = r;
		image[i+1] = g;
		image[i+2] = b;
		image[i+3] = a;
	}
}

void Bitmap::SetPixel(int x, int y, const ColorRgb& color){
	int off = (y*width+x)*4;
	image[off] = static_cast<uint8_t>(color.r*255.0f);
	image[off+1] = static_cast<uint8_t>(color.g*255.0f);
	image[off+2] = static_cast<uint8_t>(color.b*255.0f);
	image[off+3] = static_cast<uint8_t>(color.a*255.0f);
}

void Bitmap::SetPixel(int x, int y, float r, float g, float b, float a){
	int off = (y*width+x)*4;
	image[off] = static_cast<uint8_t>(r*255.0f);
	image[off+1] = static_cast<uint8_t>(g*255.0f);
	image[off+2] = static_cast<uint8_t>(b*255.0f);
	image[off+3] = static_cast<uint8_t>(a*255.0f);
}

void Bitmap::SetPixelBytes(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	int off = (y*width+x)*4;
	image[off] = r;
	image[off+1] = g;
	image[off+2] = b;
	image[off+3] = a;
}

void Bitmap::SetRenderTarget(bool b){
	if(b){
		Gfx::SetRenderTarget(this);
	} else {
		Gfx::SetRenderTarget(nullptr);
	}
}

void Bitmap::SetFilteringPoint(){
	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
void Bitmap::SetFilteringBilinear(){
	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
void Bitmap::SetFilteringTrilinear(){
	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}
void Bitmap::GenerateMipmap(){
	glGenerateMipmap(GL_TEXTURE_2D);
}

}
