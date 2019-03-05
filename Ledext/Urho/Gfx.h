#pragma once

namespace Ledlib {

class Gfx {
private:
	static float width_;
	static float height_;
	static float left_;
	static float right_;
	static float top_;
	static float bottom_;
public:
	const static float& width;
	const static float& height;
	const static float& left;
	const static float& right;
	const static float& top;
	const static float& bottom;
	Gfx();
	static void Setup();
};

}

