#pragma once

#include "ofMain.h"
#include "../Scene/Ray.h"
#include "../Scene/SceneObject.h"
#include "../Scene/SceneObjects/Light.h"

class Shader {
public:
	vector<Light *> lights;
	vector<SceneObject *> objects;
	Shader(vector<Light *> lights, vector<SceneObject *> objects);
	Shader() {};

	ofColor lambert(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse);
	ofColor phong(const glm::vec3 &p, const glm::vec3 &norm, const glm::vec3 camPos, const ofColor diffuse, const ofColor specular, float power);
	bool inShadow(const Ray &r);
};