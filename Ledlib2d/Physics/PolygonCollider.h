#pragma once

#include "Collider.h"
#include <vector>

namespace Ledlib {

class PolygonCollider : public Collider {
public:
	std::vector<glm::vec2> points;
	std::vector<int> indices;
	std::vector<std::vector<glm::vec2>> triangles;
	std::vector<std::vector<glm::vec2>> worldTriangles;

	PolygonCollider();

	virtual void OnSetup();
	virtual void OnUpdateTransform();
	virtual void OnDebugDraw();

	void AddPoint(float x, float y);
	void AddIndex(int i);
	void AddTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);

	void SetBox(float w, float h);
	void SetCircle(float radius, int numPoints);

	void GenerateTriangleStrip();
	void GenerateTriangleFan();

	void Clear();

};

}

