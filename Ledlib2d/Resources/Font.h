#pragma once

#include <string>
#include <array>

namespace Ledlib {

class Color;

class BaseFont {
protected:
    struct BaselineOffset {
        int top = 0;
        int middle = 0;
        int alphabetic = 0;
        int bottom = 0;
    } _baselineOffset;
public:
	struct Glyph {
		unsigned int valid = false;
		unsigned int textureId;
		int deviceWidth, deviceHeight;
		int left, right;
	};
	int glyphWidth, glyphHeight;
	std::array<Glyph, 256> glyphs;
	Glyph nullGlyph;

    const struct BaselineOffset& baselineOffset = _baselineOffset;
	BaseFont();
	~BaseFont();
	int GetTextWidth(const std::string& str, int spacing, bool antiKerning);
	Glyph& GetGlyph(char c);
	bool LoadBdf(const std::string& path);
};

}
