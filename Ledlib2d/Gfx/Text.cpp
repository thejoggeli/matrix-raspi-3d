#include "Text.h"
#include "OpenGL.h"
#include "../Resources/Font.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Shaders.h"
#include "OpenGL.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLES2/gl2.h>

namespace Ledlib {

static BaseFont* _font = nullptr;
static ColorRgb _textColor = ColorRgb::WHITE;

namespace Gfx {

bool antiKerning = true;
TextBaseline textBaseline = TextBaseline::TOP;
TextAlign textAlign = TextAlign::LEFT;
int textCharacterSpacing = 0;


void SetTextColor(float r, float g, float b, float a){
	_textColor.r = r; _textColor.g = g; _textColor.b = b; _textColor.a = a;
}
void SetTextColor(const ColorRgb& color){
	_textColor = color;
}

// text
void SetFont(BaseFont* font){
	_font = font;
}
void SetTextPosition(TextAlign align, TextBaseline baseline){
	textAlign = align;
	textBaseline = baseline;
}
void DrawText(std::string str, float x, float y){

	if(str.length() == 0) return;

	float cursor_x = x;
	float cursor_y = y;

	int width = _font->GetTextWidth(str, textCharacterSpacing, antiKerning);

	switch(textAlign){
	case TextAlign::LEFT:
		cursor_x += 0; break;
	case TextAlign::RIGHT:
		cursor_x += -width;
		break;
	case TextAlign::CENTER:
		cursor_x += -width/2.0f; break;
	}
	switch(textBaseline){
	case TextBaseline::TOP:
		cursor_y += -_font->baselineOffset.top; break;
	case TextBaseline::MIDDLE:
		cursor_y += -_font->baselineOffset.middle; break;
	case TextBaseline::ALPHABETIC:
		cursor_y += -_font->baselineOffset.alphabetic; break;
	case TextBaseline::BOTTOM:
		cursor_y += -_font->baselineOffset.bottom; break;
	}

	// colored texture program
	coloredTextureShader.Use();

	// set frag shader color
	glUniform4f(coloredTextureShader.color, _textColor.r, _textColor.g, _textColor.b, _textColor.a);

	for(const char& c : str) {
		BaseFont::Glyph& glyph = _font->GetGlyph(c);

		if(antiKerning){
			cursor_x -= glyph.left;
		}
		// transform
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(cursor_x, cursor_y, 0));
		model = glm::scale(model, glm::vec3(_font->glyphWidth, _font->glyphHeight, 1));
		model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0));
		glm::mat4 mvp = projectionMatrix * viewMatrix * model;
		// set vert shader vertices
		glEnableVertexAttribArray(coloredTextureShader.position);
		glBindBuffer(GL_ARRAY_BUFFER, rectangleShape.vertices_vbo);
		glVertexAttribPointer(coloredTextureShader.position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		// set vert shader uvs
		glEnableVertexAttribArray(coloredTextureShader.texCoords);
		glBindBuffer(GL_ARRAY_BUFFER, rectangleShape.uvs_vbo);
		glVertexAttribPointer(coloredTextureShader.texCoords, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		// set vert shader mvp
		glUniformMatrix4fv(coloredTextureShader.mvp, 1, GL_FALSE, glm::value_ptr(mvp));
		// set frag shader texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glyph.textureId);
		glUniform1i(coloredTextureShader.texture, 0);
		// draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//	cursor_x += glyph.deviceWidth + textCharacterSpacing;
		if(antiKerning){
			cursor_x += glyph.left + (glyph.right-glyph.left) + 2 + textCharacterSpacing;
		} else {
			cursor_x += glyph.deviceWidth + textCharacterSpacing;
		}

	}
}

}
}
