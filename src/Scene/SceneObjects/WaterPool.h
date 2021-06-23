#pragma once

#include "ofMain.h"
#include "../SceneObject.h"
 
//************************************ WaterPool Class *********************************
// DEPRECATED //
class WaterPool : public SceneObject {
public:
	ofPlanePrimitive plane;
	glm::vec3 normal;
	float radius = 1.0;
	float width = 20;
	float height = 20;

	WaterPool(glm::vec3 p, float r, string name, ofColor diffuse = ofColor::lightGray);
	WaterPool() {}
	~WaterPool() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor);
	void draw();
	float sdf(glm::vec3 p, int amp, int octaves);
};
