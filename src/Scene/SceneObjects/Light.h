#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

//Light class with an intensity variable
class Light : public SceneObject {
public:
	float intensity = 100;
	float radius = 0.1;

	Light(glm::vec3 p, float i) {
		position = p;
		intensity = i;
	}

	Light() {}
	void draw() { ofDrawSphere(position, radius); };
};