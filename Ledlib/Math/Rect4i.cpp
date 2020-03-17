#include "Rect4i.h"

namespace Ledlib {

Rect4i::Rect4i(){}
Rect4i::Rect4i(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
void Rect4i::Set(int x, int y, int w, int h){
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

}
