#pragma once

#include "ofMain.h"
#include "Light.h"

/** 
 * Spotlight class. Inherits from Light and has variables for angles that constraint
 * the light into a cone with some falloff light to provide smoother edges.
 **/
class SpotLight : public Light {
public:
	glm::vec3 direction;
	float totalWidth;
	float falloffStart;
	float heightRef = 6.0f;

	SpotLight(glm::vec3 p, glm::vec3 d, float angle, float foAngle, string name);
	SpotLight() {};
	~SpotLight() {};

	void draw();
	float falloff(float spotAngle);
};