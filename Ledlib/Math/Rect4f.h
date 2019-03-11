#pragma once

namespace Ledlib {

class Rect4f {
public:
	float x, y, w, h;
	Rect4f();
	Rect4f(float x, float y, float w, float h);
	void Set(float x, float y, float w, float h);
};

}
