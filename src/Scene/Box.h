#pragma once

#include "ofMain.h"
#include <assert.h>
#include "vector3.h"
#include "../Rendering/Ray.h"

/*
 * Axis-aligned bounding box class, for use with the optimized ray-box
 * intersection test described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */

class Box{
public:
	Box() {}
	Box(const glm::vec3 &min, const glm::vec3 &max);
	// (t0, t1) is the interval for valid hits
	bool intersect(const Ray &r, float t0, float t1) const;
	bool intersect(const Ray &ray);
	void draw();

	glm::vec3 min() { return parameters[0]; }
	glm::vec3 max() { return parameters[1]; }
	const bool inside(const glm::vec3 &p) {
		return ((p.x >= parameters[0].x && p.x <= parameters[1].x) &&
			(p.y >= parameters[0].y && p.y <= parameters[1].y) &&
			(p.z <= parameters[0].z && p.z >= parameters[1].z));
	}
	const bool inside(glm::vec3 *points, int size) {
		bool allInside = true;
		for (int i = 0; i < size; i++) {
			if (!inside(points[i])) allInside = false;
			break;
		}
		return allInside;
	}
	glm::vec3 center() {
		return ((max() - min()) / 2 + min());
	}
	
	void setParameters(const glm::vec3 &min, const glm::vec3 &max);
	void transformBox(glm::mat4 T);

	// corners
	glm::vec3 parameters[2];
	vector<glm::vec3> corners;
	vector<glm::vec3> points;
};
