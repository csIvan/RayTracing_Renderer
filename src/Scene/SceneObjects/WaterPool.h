#pragma once

#include "ofMain.h"
#include "../SceneObject.h"
 
//************************************ WaterPool Class *********************************
class WaterPool : public SceneObject {
public:
	float radius = 1.0;

	WaterPool(glm::vec3 p, float r, string name, ofColor diffuse = ofColor::lightGray);
	WaterPool() {}

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	void draw();
	float sdf(glm::vec3 p, int amp, int octaves);
};
