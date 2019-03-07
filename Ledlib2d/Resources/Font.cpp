#include "Font.h"
#include "Ledlib/Log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <GLES2/gl2.h>

using namespace std;


namespace Ledlib {

BaseFont::BaseFont(){}

BaseFont::~BaseFont(){}

BaseFont::Glyph& BaseFont::GetGlyph(char c){
	Glyph& glyph = glyphs[static_cast<uint8_t>(c)];
	if(!glyph.valid){
		return nullGlyph;
	}
	return glyph;
}

bool BaseFont::LoadBdf(const std::string &path){

	glyphs.fill(Glyph());

	char buffer[1024];
	FILE *f = fopen(path.c_str(), "r");
	if(!f){
		Log(LOG_ERROR, "Font", "Couldn't open file: " + path);
		fclose(f);
		return false;
	}
	int count = 0;
	int state = 0; // 0 = anything; 10 = charhead; 20 = charbitmap
	unsigned int encoding;
	int yPos = 0;
	char s1[256];
	int bitshift = 0;
	int capHeight = 0;
	int xHeight = 0;

	int bbox_w = 0, bbox_h = 0, bbox_xoff = 0, bbox_yoff = 0;
	int glyph_w = 0, glyph_h = 0, glyph_xoff = 0, glyph_yoff = 0;

	std::vector<uint32_t> pixels;

	int glyph_left = 0, glyph_right = 0;

	Glyph* glyph = nullptr;
	while (fgets(buffer, sizeof(buffer), f)) {
		if(state == 0){
			if (sscanf(buffer, "FONTBOUNDINGBOX %d %d %d %d", &bbox_w, &bbox_h, &bbox_xoff, &bbox_yoff) == 4){
				glyphWidth = bbox_w;
				glyphHeight = bbox_h;
			} else if(sscanf(buffer, "STARTCHAR %s", s1) == 1){
				state = 10;
			} else if(sscanf(buffer, "CAP_HEIGHT %d", &capHeight) == 1){
				// done
			} else if(sscanf(buffer, "X_HEIGHT %d", &xHeight) == 1){
				// done
			}
		} else if(state == 10){
			if (sscanf(buffer, "ENCODING %d", &encoding) == 1){
				if(encoding < 256){
					if(glyph != nullptr){
						Log(LOG_ERROR, "Font", "Glyph was not null");
					}
					glyph = &glyphs[encoding];
				} else {
					state = 0;
				}
			} else if (sscanf(buffer, "BBX %d %d %d %d", &glyph_w, &glyph_h, &glyph_xoff, &glyph_yoff) == 4){
				pixels.assign(static_cast<uint32_t>(bbox_w*bbox_h), 0x0);
			} else if (sscanf(buffer, "DWIDTH %d %d", &(glyph->deviceWidth), &(glyph->deviceHeight)) == 2){
				// done
			} else if (strncmp(buffer, "BITMAP", strlen("BITMAP")) == 0){
				state = 20;
				yPos = 0;
				bitshift = 8 * (static_cast<int>(sizeof(uint32_t)) - ((glyph_w + 7) / 8));
				glyph_right = 0;
				glyph_left = bbox_w-1;
			}
		} else if(state == 20){
			if(yPos < glyph_h){
				uint32_t bits;
				if(sscanf(buffer, "%x", &bits) == 1){
					bits <<= (bitshift+bbox_xoff-glyph_xoff);
					int y = yPos + (bbox_h - glyph_h) + bbox_yoff - glyph_yoff;
					int offset = y * bbox_w;
					uint32_t mask = 0x80000000;
					for(int i = 0; i < bbox_w; i++){
						if(mask&bits){
							pixels[offset+i] = 0xFFFFFFFF;
							if(i < glyph_left) glyph_left = i;
							if(i > glyph_right) glyph_right = i;
						}
						mask >>= 1u;
					}
				}
				yPos++;
			} else if(strncmp(buffer, "ENDCHAR", strlen("ENDCHAR")) == 0){
				if(yPos != glyph_h){
					Log(LOG_ERROR, "Font", "ENDCHAR and glyph height don't match");
				}
				state = 0;
				glGenTextures(1, &glyph->textureId);
				glBindTexture(GL_TEXTURE_2D, glyph->textureId);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bbox_w, bbox_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glyph->valid = true;
				if(glyph_right < glyph_left){
					glyph_left = 0;
					glyph_right = glyph->deviceWidth-1;
				}
				glyph->left = glyph_left;
				glyph->right = glyph_right;
				glyph = nullptr;
				count++;
			} else {
				Log(LOG_ERROR, "Font", "invalid state #1");
			}
		}
	}
	fclose(f);

	Log(LOG_DEBUG, "Font", iLog << count << " Glyphs loaded");

	// baseline offsets
	_baselineOffset.top = capHeight-bbox_yoff;
	_baselineOffset.middle = capHeight/2-bbox_yoff;
	_baselineOffset.alphabetic = -bbox_yoff;
	_baselineOffset.bottom = 0;

	// nullglyph texture
	nullGlyph.valid = false;
	nullGlyph.left = 0;
	nullGlyph.right = bbox_w-1;
	pixels.assign(bbox_w*bbox_h, 0xFFFFFFFF);
	glGenTextures(1, &nullGlyph.textureId);
	glBindTexture(GL_TEXTURE_2D, nullGlyph.textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bbox_w, bbox_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return true;
}

int BaseFont::GetTextWidth(const string& str, int spacing, bool antiKerning){
	if(str.length() == 0) return 0;
	Glyph* glyph;
	int width = 0;
	if(antiKerning){
		for(const char& c : str) {
			glyph = &GetGlyph(c);
			width += glyph->right-glyph->left+2+spacing;
		}
		width -= spacing+1;
	} else {
		for(const char& c : str) {
			glyph = &GetGlyph(c);
			width = width + glyph->deviceWidth + spacing;
		}
		width -= spacing;
	}
	return width;
}
}
