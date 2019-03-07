#pragma once

#include <glm/fwd.hpp>

namespace Ledlib {
namespace Gfx {

extern const glm::mat4& projectionMatrix;
extern const glm::mat4& viewMatrix;

void Save();
void Restore();
void Transform(const glm::mat4& m);
void Translate(float x, float y, float z = 0);
void Translate(const glm::vec3& v);
void Scale(float x, float y, float z = 1);
void Rotate(float z);
void Rotate(float angle, const glm::vec3& axis);
void SetProjectionMatrix(const glm::mat4& m);
void Rotate(const glm::mat4& m);

}
}

