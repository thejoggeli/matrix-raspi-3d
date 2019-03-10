#pragma once

#include <string>
#include <vector>

namespace Ledlib {
class ColorRgb;

class Bitmap {
protected:
	uint32_t _width, _height, _numPixels;
public:
	bool created = false;
	unsigned int textureId;
	std::vector<uint8_t> image;
	const uint32_t& width = _width;
	const uint32_t& height = _height;
	const uint32_t& numPixels = _numPixels;
	Bitmap();
	virtual ~Bitmap();
	bool CreateEmpty(uint32_t width, uint32_t height);
	bool CreateFromPng(const std::string& path);
	void Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	void SetPixel(int x, int y, const ColorRgb& color);
	void Update();
};

}
