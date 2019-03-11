#pragma once

namespace Ledlib {

class Rect4i {
public:
	int x, y, w, h;
	Rect4i();
	Rect4i(int x, int y, int w, int h);
	void Set(int x, int y, int w, int h);
};

}
