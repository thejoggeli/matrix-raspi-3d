#pragma once

#include "Shader.h"
#include <glm/fwd.hpp>
#include <vector>

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
	std::vector<int> argsf;
	std::vector<int> argsi;
	ShaderBox();
	virtual ~ShaderBox();
	void LoadFile(const char* name);
	void SetCameraPosition(const glm::vec3& position);
	void SetCameraRotation(const glm::quat& rotation);
	void SetCameraScale(const glm::vec3& scale);
	void AddArgs4f();
	void AddArgs1i();
	void SetArgs4f(int index, const glm::vec4 values);
	void SetArgs1i(int index, int value);
};

namespace Gfx {
void DrawShaderBox(ShaderBox& box, float x, float y);
void DrawShaderBox(ShaderBox& box, float x, float y, float width, float height);
}
}

