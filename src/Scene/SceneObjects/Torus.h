#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

//************************************** Torus Class *********************************
class Torus : public SceneObject {
public:
	ofMaterial material;
	float innerRadius;
	float outerRadius;
	float angle = -45.0f;
	glm::vec3 axisR = glm::vec3(1, 0, 0);

	int numc = 20;
	int numt = 35;

	Torus() {};
	Torus(glm::vec3 p, float l1, float l2, ofColor diffuse = ofColor::azure);
	Torus(glm::vec3 p, float l1, float l2, float a, glm::vec3 r, ofColor diffuse = ofColor::azure);

	void draw();
	float sdf(glm::vec3 p1);
};
