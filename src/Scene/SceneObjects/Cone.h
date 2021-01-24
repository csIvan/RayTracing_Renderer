#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

//************************************ Cone Class ***************************************
class Cone : public SceneObject {
public:
	ofMaterial material;
	float height = 2.0f;
	float radius = 0.25f;

	Cone(glm::vec3 p, float h, float r, string name, ofColor diffuse = ofColor::lightGray);
	Cone() {};
	~Cone() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	void draw();
	float sdf(const glm::vec3 p);
};