#pragma once

#include "ofMain.h"

// Struct container for details of ray-object interaction
struct HitInfo {
	bool hit = false;
	glm::vec3 point;
	glm::vec3 normal;
	ofColor surfaceColor;
};

// - General Purpose Ray Class -
class Ray {
public:
	glm::vec3 p, d;		// point and direction
	glm::vec3 inv_dir;	// inverse of the ray's direction
	int sign[3];

	Ray(glm::vec3 p, glm::vec3 d) : p(p), d(d) {
		inv_dir = 1.0f / d;
		sign[0] = (inv_dir.x < 0);
		sign[1] = (inv_dir.y < 0);
		sign[2] = (inv_dir.z > 0);
	}

	void draw(float t) {
		ofDrawLine(p, p + t * d);
	}

	glm::vec3 evalPoint(float t) const {
		return (p + t * d);
	}
};
