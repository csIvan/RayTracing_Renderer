#pragma once

#include "ofMain.h"
#include "../Scene/Ray.h"

class Shader {
public:
	Shader();

	ofColor lambert(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse);
	ofColor phong(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse, const ofColor specular, float power);
	bool inShadow(const Ray &r);
};