#pragma once

#include "ofMain.h"
#include "../Light.h"

//Spotlight class. Inherits from Light and has variables for angles that constraint
// the light into a cone with some falloff light to provide smoother edges.
class Spotlight : public Light {
public:
	glm::vec3 direction;
	float lightAngle;
	float falloffAngle;

	Spotlight(glm::vec3 d, float angle, float foAngle) {
		direction = d;
		lightAngle = glm::radians(angle);
		falloffAngle = glm::radians(foAngle);
	}

	Spotlight() {
		direction = glm::vec3(0.0f, -1.0f, 0.0f);
		lightAngle = glm::radians(10.0);
		falloffAngle = lightAngle - .1;
	}
	float falloff(float spotAngle);
};