#include "Shader.h"
#include "OpenGL.h"
#include "Ledlib/Log.h"
#include "Ledlib/Util/Strings.h"
#include <GLES2/gl2.h>

namespace Ledlib {

static int currentShaderId = -1;

static unsigned int CreateShader(const char* src, GLenum type);
static unsigned int CreateShaderProgram(unsigned int vert, unsigned int frag);

Shader::Shader(){}
Shader::~Shader(){}
void Shader::Use(){
	if(currentShaderId == id){
		return;
	}
	currentShaderId = id;
	glUseProgram(id);
}
void Shader::Compile(const char *vert, const char *frag){
	if(compiled){
		glDeleteProgram(id);
	}
	GLuint vert_s = CreateShader(vert, GL_VERTEX_SHADER);
	GLuint frag_s = CreateShader(frag, GL_FRAGMENT_SHADER);
	this->id = CreateShaderProgram(vert_s, frag_s);
	glDeleteShader(vert_s);
	glDeleteShader(frag_s);
	compiled = true;
}

static unsigned int CreateShader(const char* src, GLenum type){
	// vertex shader
	unsigned int shader = glCreateShader(type);
	if(Gfx::PrintOpenGLError("glCreateShader")) return false;

	glShaderSource(shader, 1, &src, nullptr);
	if(Gfx::PrintOpenGLError("glShaderSource")) return false;

	glCompileShader(shader);
	if(Gfx::PrintOpenGLError("glCompileShader")) return false;

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
		Gfx::PrintOpenGLError("glCreateProgram");
		return false;
	}
	if(Gfx::PrintOpenGLError("glCreateProgram")) return false;

	glAttachShader(program, vert);
	if(Gfx::PrintOpenGLError("glAttachShader")) return false;

	glAttachShader(program, frag);
	if(Gfx::PrintOpenGLError("glAttachShader")) return false;

	glLinkProgram(program);
	if(Gfx::PrintOpenGLError("glLinkProgram")) return false;

	glUseProgram(program);
	if(Gfx::PrintOpenGLError("glUseProgram")) return false;

	return program;
}
namespace Gfx {}
}
