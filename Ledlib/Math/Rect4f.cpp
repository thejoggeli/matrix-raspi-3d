#include "Rect4f.h"

namespace Ledlib {

Rect4f::Rect4f(){}
Rect4f::Rect4f(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
void Rect4f::Set(float x, float y, float w, float h){
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

}
