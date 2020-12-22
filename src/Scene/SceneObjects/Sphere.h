#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

// General purpose sphere  (assume parametric)
class Sphere : public SceneObject {

public:
	Sphere(glm::vec3 p, float r, ofColor diffuse = ofColor::lightGray) { position = p; radius = r; diffuseColor = diffuse; }
	Sphere() {}
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}
	float sdf(glm::vec3 p) {
		float distance = glm::distance(p, position) - radius;
		return  distance;
	}
	void draw() {
		ofDrawSphere(position, radius);
	}

	float radius = 1.0;
};
