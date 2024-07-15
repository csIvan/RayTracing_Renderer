#pragma once

#include "ofMain.h"
#include "vector3.h"
#include <assert.h>
#include "../Rendering/Ray.h"


// - Axis-aligned bounding box (AABB) class -
class AABB {
private:
	glm::vec3 bounds[2];
	vector<glm::vec3> corners;

public:
	AABB() {};
	AABB(const glm::vec3 &min, const glm::vec3 &max);

	bool intersect(const Ray &ray) const;
	void draw();

	// Transforms the AABB using a transformation matrix
	void transformBox(const glm::mat4 &T);

	// Sets the parameters of the AABB with the new min and max bounds
	void setParameters(const glm::vec3 &min, const glm::vec3 &max);

	// Getters and utility functions
	glm::vec3 getMin() const { return bounds[0]; }
	glm::vec3 getMax() const { return bounds[1]; }
	glm::vec3 getCenter() const {
		return ((getMax() - getMin()) / 2 + getMin());
	}
	const bool isInside(const glm::vec3 &p) const {
		return ((p.x >= bounds[0].x && p.x <= bounds[1].x) &&
			(p.y >= bounds[0].y && p.y <= bounds[1].y) &&
			(p.z <= bounds[0].z && p.z >= bounds[1].z));
	}
};