#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

//************************************ Cube Class ***************************************
class Cube : public SceneObject {
public:
	float side = 1.0f;

	Cube(glm::vec3 p, float s, string name, ofColor diffuse = ofColor::lightGray);
	Cube() {};
	~Cube() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	void draw();
	float sdf(const glm::vec3 p);
};