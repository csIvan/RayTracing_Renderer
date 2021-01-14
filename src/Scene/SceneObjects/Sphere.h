#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

// General purpose sphere  (assume parametric)
class Sphere : public SceneObject {
public:
	ofMaterial material;
	float radius = 1.0;

	Sphere(glm::vec3 p, float r, string name, ofColor diffuse = ofColor::lightGray);
	Sphere() {}
	~Sphere() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	void draw();
	float sdf(glm::vec3 p);
};
