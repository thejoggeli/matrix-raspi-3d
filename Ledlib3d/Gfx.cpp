#include "Gfx.h"
#include "Ledlib/Display/DisplayManager.h"

namespace Ledlib {

float Gfx::width_;
float Gfx::height_;
float Gfx::left_;
float Gfx::right_;
float Gfx::top_;
float Gfx::bottom_;

const float& Gfx::width = Gfx::width_;
const float& Gfx::height = Gfx::height_;
const float& Gfx::left = Gfx::left_;
const float& Gfx::right = Gfx::right_;
const float& Gfx::top = Gfx::top_;
const float& Gfx::bottom = Gfx::bottom_;

Gfx::Gfx(){}

void Gfx::Setup(){
	width_ = DisplayManager::width;
	height_ = DisplayManager::height;
	left_ = -width_/2.0f;
	right_ = width_/2.0f;
	top_ = width_/2.0f;
	bottom_ = -width_/2.0f;
}

}

