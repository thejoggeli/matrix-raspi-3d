#pragma once

#include "Shader.h"
#include <glm/fwd.hpp>
#include <unordered_map>

namespace Ledlib {

class ShaderBox : public Shader {
public:
	int position;
	int mvp;
	int coords;
	int time;
	int view;
	int cameraPosition;
	int cameraRotation;
	int cameraRotationInverse;
	int cameraScale;
	std::unordered_map<std::string, int> argsf;
	std::unordered_map<std::string, int> argsi;
	ShaderBox();
	virtual ~ShaderBox();
	void LoadFile(const char* name);
	void SetCameraPosition(const glm::vec3& position);
	void SetCameraRotation(const glm::quat& rotation);
	void SetCameraScale(const glm::vec3& scale);
	void AddArgs4f(const std::string& name);
	void AddArgs1i(const std::string& name);
	void SetArgs4f(const std::string& name, const glm::vec4 values);
	void SetArgs1i(const std::string& name, int value);
};

namespace Gfx {
void DrawShaderBox(ShaderBox& box, float x, float y);
void DrawShaderBox(ShaderBox& box, float x, float y, float width, float height);
}
}

