#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

//************************************ Cube Class ***************************************
class Cube : public SceneObject {
public:
	Cube(glm::vec3 p, float s, ofColor diffuse = ofColor::lightGray) { position = p; side = s; diffuseColor = diffuse; }
	Cube() {}
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, side, point, normal));
	}
	float sdf(const glm::vec3 p) {
		glm::vec3 q = abs(p) - side;
		return MIN(MAX(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0f));
	}

	void draw() {
		ofDrawSphere(position, side);
	}


	float side = 1.0f;
};