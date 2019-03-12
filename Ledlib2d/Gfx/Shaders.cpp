#include "Shaders.h"
#include <GLES2/gl2.h>

#define STRINGIFY(x) #x

namespace Ledlib {
namespace Gfx {

static const char* vert_vanilla_src = STRINGIFY(
	attribute vec3 a_position;
	uniform mat4 u_mvpMatrix;
	void main() {
		gl_Position = u_mvpMatrix*vec4(a_position,1.0);
	}
);

static const char* frag_solid_color_src = STRINGIFY(
	uniform vec4 u_color;
	void main() {
		gl_FragColor = u_color;
	}
);

static const char* vert_basic_texture_src = STRINGIFY(
	attribute vec3 a_position;
	attribute vec2 a_textureCoordinate;
	varying vec2 v_textureCoordinate;
	uniform mat4 u_mvpMatrix;
	void main() {
		gl_Position = u_mvpMatrix*vec4(a_position,1.0);
		v_textureCoordinate = a_textureCoordinate;
	}
);

static const char* frag_colored_texture_src = STRINGIFY(
	varying vec2 v_textureCoordinate;
	uniform vec4 u_color;
	uniform sampler2D s_texture;
	void main() {
	//	gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
		gl_FragColor = u_color*texture2D(s_texture, v_textureCoordinate);
	//	gl_FragColor = texture2D(s_texture, v_textureCoordinate);
	}
);

SolidColorShader solidColorShader;
ColoredTextureShader coloredTextureShader;

void InitShaders(){
	// solid color
	solidColorShader.Compile(vert_vanilla_src, frag_solid_color_src);
	solidColorShader.position = glGetAttribLocation(solidColorShader.id, "a_position");
	solidColorShader.mvp = glGetUniformLocation(solidColorShader.id, "u_mvpMatrix");
	solidColorShader.color = glGetUniformLocation(solidColorShader.id, "u_color");
	// colored texture
	coloredTextureShader.Compile(vert_basic_texture_src, frag_colored_texture_src);
	coloredTextureShader.position = glGetAttribLocation(coloredTextureShader.id, "a_position");
	coloredTextureShader.mvp = glGetUniformLocation(coloredTextureShader.id, "u_mvpMatrix");
	coloredTextureShader.color = glGetUniformLocation(coloredTextureShader.id, "u_color");
	coloredTextureShader.texture = glGetUniformLocation(coloredTextureShader.id, "s_texture");
	coloredTextureShader.texCoords = glGetAttribLocation(coloredTextureShader.id, "a_textureCoordinate");
}

}
}

