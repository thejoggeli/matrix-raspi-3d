#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Ledlib {
class ColorRgb;

class Bitmap {
protected:
	uint32_t _width, _height, _numPixels;
	bool _renderable = false;
public:
	Bitmap();
	~Bitmap();
	unsigned int textureId;
	unsigned int framebufferId;
	std::vector<uint8_t> image;
	const uint32_t& width = _width;
	const uint32_t& height = _height;
	const uint32_t& numPixels = _numPixels;
	const bool& renderable = _renderable;
	static std::shared_ptr<Bitmap> CreateEmpty(uint32_t width, uint32_t height);
	static std::shared_ptr<Bitmap> CreateRenderable(uint32_t width, uint32_t height);
	static std::shared_ptr<Bitmap> CreateFromPng(const std::string& path);

	void Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	void SetPixel(int x, int y, const ColorRgb& color);
	void Update();

	void SetRenderTarget(bool b);

};

}
