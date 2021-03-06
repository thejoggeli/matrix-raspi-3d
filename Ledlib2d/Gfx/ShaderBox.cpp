#include "ShaderBox.h"
#include <GLES2/gl2.h>
#include <fstream>
#include <string>
#include "Transform.h"
#include "OpenGL.h"
#include "Gfx.h"
#include "Ledlib/Time.h"
#include "Ledlib/Log.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Resources/ResourceManager.h"
#include "../Resources/Bitmap.h"


#define STRINGIFY(x) #x

namespace Ledlib {

static const char* vert_shader_src = STRINGIFY(
	attribute vec3 a_position;
	attribute vec2 a_coords;
	uniform mat4 u_mvp;
	varying vec2 iCoords;
	void main() {
		gl_Position = u_mvp*vec4(a_position,1.0);
		iCoords = a_coords;
	}
);

ShaderBox::ShaderBox(){}

ShaderBox::~ShaderBox(){
	if(compiled){
		glDeleteProgram(id);
	}
}

void ShaderBox::LoadFile(const char* file){
	std::string path = ResourceManager::resourcesPath + "/shaders/" + file;
	std::ifstream ifs(path.c_str());
	if(!ifs.good()){
		Log(LOG_ERROR, "ShaderBox", "can't read file: " + path);
		return;
	}
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
//	Log(content);
	Compile(vert_shader_src, content.c_str());
	Log(LOG_DEBUG, "ShaderBox", iLog << "Compiled (id=" << id << ") " << file);
	position = glGetAttribLocation(id, "a_position");
	mvp = glGetUniformLocation(id, "u_mvp");
	coords = glGetAttribLocation(id, "a_coords");
	time = glGetUniformLocation(id, "iTime");
	rect = glGetUniformLocation(id, "iRect");
	cameraPosition = glGetUniformLocation(id, "iCamPos");
	cameraRotation = glGetUniformLocation(id, "iCamRot");
	cameraRotationInverse = glGetUniformLocation(id, "iCamRotInv");
	cameraScale = glGetUniformLocation(id, "iCamScale");
}
void ShaderBox::AddArgs1i(const std::string& name){
	argsi[name] = glGetUniformLocation(id, name.c_str());
}
void ShaderBox::AddArgs4f(const std::string& name){
	argsf[name] = glGetUniformLocation(id, name.c_str());
}
void ShaderBox::AddArgsBitmap(const std::string &name){
	argst[name] = glGetUniformLocation(id, name.c_str());
}
void ShaderBox::SetArgs1i(const std::string& name, int value){
	Use();
	glUniform1i(argsi[name], value);
}
void ShaderBox::SetArgs4f(const std::string& name, const glm::vec4 values){
	Use();
	glUniform4fv(argsf[name], 1, glm::value_ptr(values));
}
void ShaderBox::SetArgsBitmap(const std::string &name, const Bitmap* bitmap, int unit){
	Use();
	switch(unit){
		case 0: glActiveTexture(GL_TEXTURE0); break;
		case 1: glActiveTexture(GL_TEXTURE1); break;
		case 2: glActiveTexture(GL_TEXTURE2); break;
		case 3: glActiveTexture(GL_TEXTURE3); break;
		default: return;
	}
	glBindTexture(GL_TEXTURE_2D, bitmap->textureId);
	glUniform1f(argst[name], unit);
}

void ShaderBox::SetCameraPosition(const glm::vec3& position){
	Use();
	glUniform3fv(cameraPosition, 1, glm::value_ptr(position));
}
void ShaderBox::SetCameraRotation(const glm::quat& rotation){
	Use();
	glm::mat3 rot = glm::toMat3(rotation);
	glUniformMatrix3fv(cameraRotation, 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix3fv(cameraRotationInverse, 1, GL_FALSE, glm::value_ptr(glm::inverse(rot)));
}
void ShaderBox::SetCameraScale(const glm::vec3& scale){
	Use();
	glUniform3fv(cameraScale, 1, glm::value_ptr(scale));
}

namespace Gfx {

void DrawShaderBox(ShaderBox& box, float x, float y){
	Gfx::DrawShaderBox(box, x, y, Gfx::width, Gfx::height);
}

void DrawShaderBox(ShaderBox& box, float x, float y, float width, float height){
	box.Use();
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
	model = glm::scale(model, glm::vec3(width, -height, 1.0f));
	glm::mat4 mvp = projectionMatrix * viewMatrix * model;
	// mvp
	glUniformMatrix4fv(box.mvp, 1, GL_FALSE, glm::value_ptr(mvp));
	// vertices
	glEnableVertexAttribArray(box.position);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleShape.vertices_vbo);
	glVertexAttribPointer(box.position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	// uvs
	glEnableVertexAttribArray(box.coords);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleShape.uvs_vbo);
	glVertexAttribPointer(box.coords, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	// view
	glm::vec3 rect(width, height, width/height);
	glUniform3fv(box.rect, 1, glm::value_ptr(rect));
	// time
	glUniform1f(box.time, Time::sinceStart);
	// draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

}
}
