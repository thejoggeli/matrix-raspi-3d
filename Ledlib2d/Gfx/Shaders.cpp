#include "Shaders.h"
#include "OpenGL.h"
#include "Ledlib/Log.h"
#include "Ledlib/Util/Strings.h"
#include <GLES2/gl2.h>
#include <vector>

#define STRINGIFY(x) #x

namespace Ledlib {
namespace Gfx {

static int currentShaderId = -1;

Shader::Shader(){}
Shader::~Shader(){}
void Shader::Use(){
	if(currentShaderId == id){
		return;
	}
	currentShaderId = id;
	glUseProgram(id);
}

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

static unsigned int CreateShader(const char* src, GLenum type){
	// vertex shader
	unsigned int shader = glCreateShader(type);
	if(PrintOpenGLError("glCreateShader")) return false;

	glShaderSource(shader, 1, &src, nullptr);
	if(PrintOpenGLError("glShaderSource")) return false;

	glCompileShader(shader);
	if(PrintOpenGLError("glCompileShader")) return false;

	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE){
		Log(LOG_ERROR, "Gfx/OpenGL", "Failed to compile shader: ");
	//	Log(LOG_ERROR, "Gfx/OpenGL", src);
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		std::string log;
		for (int i = 0; i < errorLog.size(); i++) {
			log += errorLog[i];
		}
		log = Strings::Trim(log);
		Log(LOG_ERROR, "Gfx/OpenGL", log);
	}
	return shader;
}

static unsigned int CreateShaderProgram(unsigned int vert, unsigned int frag){
	// shader program
	unsigned int program = glCreateProgram();
	if(!program){
		PrintOpenGLError("glCreateProgram");
		return false;
	}
	if(PrintOpenGLError("glCreateProgram")) return false;

	glAttachShader(program, vert);
	if(PrintOpenGLError("glAttachShader")) return false;

	glAttachShader(program, frag);
	if(PrintOpenGLError("glAttachShader")) return false;

	glLinkProgram(program);
	if(PrintOpenGLError("glLinkProgram")) return false;

	glUseProgram(program);
	if(PrintOpenGLError("glUseProgram")) return false;

	return program;
}

SolidColorShader solidColorShader;
ColoredTextureShader coloredTextureShader;

void InitShaders(){
	// shaders
	GLuint vertVanillaShader = CreateShader(vert_vanilla_src, GL_VERTEX_SHADER);
	GLuint vertBasicTextureShader = CreateShader(vert_basic_texture_src, GL_VERTEX_SHADER);
	GLuint fragSolidColorShader = CreateShader(frag_solid_color_src, GL_FRAGMENT_SHADER);
	GLuint fragColoredTextureShader = CreateShader(frag_colored_texture_src, GL_FRAGMENT_SHADER);

	// shader programs
	// solid color
	solidColorShader.id = CreateShaderProgram(vertVanillaShader, fragSolidColorShader);
	solidColorShader.position = glGetAttribLocation(solidColorShader.id, "a_position");
	solidColorShader.mvp = glGetUniformLocation(solidColorShader.id, "u_mvpMatrix");
	solidColorShader.color = glGetUniformLocation(solidColorShader.id, "u_color");

	// colored texture
	coloredTextureShader.id = CreateShaderProgram(vertBasicTextureShader, fragColoredTextureShader);
	coloredTextureShader.position = glGetAttribLocation(coloredTextureShader.id, "a_position");
	coloredTextureShader.mvp = glGetUniformLocation(coloredTextureShader.id, "u_mvpMatrix");
	coloredTextureShader.color = glGetUniformLocation(coloredTextureShader.id, "u_color");
	coloredTextureShader.texture = glGetUniformLocation(coloredTextureShader.id, "s_texture");
	coloredTextureShader.texCoords = glGetAttribLocation(coloredTextureShader.id, "a_textureCoordinate");

}

}
}

