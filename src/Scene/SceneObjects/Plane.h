#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

//  General purpose plane 
class Plane : public SceneObject {
public:
	ofPlanePrimitive plane;
	glm::vec3 normal;
	float width = 20;
	float height = 20;

	Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse = ofColor::darkOliveGreen, float w = 50, float h = 50);
	Plane();

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	void draw();
	float sdf(glm::vec3  p);

	glm::vec3 getNormal(const glm::vec3 &p);
};