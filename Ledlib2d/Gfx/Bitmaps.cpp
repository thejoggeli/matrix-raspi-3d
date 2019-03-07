#include "Bitmaps.h"
#include "../Resources/Bitmap.h"
#include "Shaders.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Transform.h"
#include "OpenGL.h"
#include <GLES2/gl2.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Ledlib {

static ColorRgb _bitmapColor;

namespace Gfx {

// colors
void SetBitmapColor(float r, float g, float b, float a){
	_bitmapColor.r = r; _bitmapColor.g = g; _bitmapColor.b = b; _bitmapColor.a = a;
}
void SetBitmapColor(const ColorRgb& color){
	_bitmapColor = color;
}

// bitmap
void DrawBitmap(const Bitmap* bitmap, float x, float y, float rotation){
	DrawBitmap(bitmap, x, y, bitmap->width, bitmap->height, rotation);
}
void DrawBitmap(const Bitmap* bitmap, float x, float y, float w, float h, float rotation){
	// transformation
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
	if(rotation != 0.0f){
		model = glm::rotate(model, rotation, glm::vec3(0, 0, 1));
	}
	model = glm::scale(model, glm::vec3(w, h, 1.0f));
	glm::mat4 mvp = projectionMatrix * viewMatrix * model;
	// select colored texture program
	coloredTextureShader.Use();
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
	// set frag shader color
	glUniform4f(coloredTextureShader.color, _bitmapColor.r, _bitmapColor.g, _bitmapColor.b, _bitmapColor.a);
	// set frag shader texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bitmap->textureId);
	glUniform1i(coloredTextureShader.texture, 0);
	// draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// void DrawBitmap(const Bitmap* bitmap, float x, float y, float w, float h, const Rect4f* rsrc, float rotation){}

}
}

