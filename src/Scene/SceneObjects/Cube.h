#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

//************************************ Cube Class ***************************************
class Cube : public SceneObject {
public:
	float side = 1.0f;
	float angle = 45.0f;
	glm::vec3 axisR = glm::vec3(-1, 0, 0);

	Cube(glm::vec3 p, float s, ofColor diffuse = ofColor::lightGray);
	Cube() {}

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	void draw();
	float sdf(const glm::vec3 p);
};