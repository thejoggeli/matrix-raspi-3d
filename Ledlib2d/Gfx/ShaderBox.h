#pragma once

#include "Shader.h"
#include <glm/fwd.hpp>

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
	ShaderBox();
	virtual ~ShaderBox();
	void LoadFile(const char* name);
	void SetCameraPosition(const glm::vec3& position);
	void SetCameraRotation(const glm::quat& rotation);
	void SetCameraScale(const glm::vec3& scale);
};

namespace Gfx {
void DrawShaderBox(ShaderBox& box, float x, float y);
void DrawShaderBox(ShaderBox& box, float x, float y, float width, float height);
}
}

