#pragma once

#include "ofMain.h"
#include "Light.h"

class AreaLight : public Light {
public:
	ofPlanePrimitive plane;
	glm::vec3 direction;
	float height = 1;
	float width = 1;
	glm::vec3 corner, uv, vv, uvec, vvec;
	int usteps, vsteps, samples;
	vector<glm::vec3> lightPositions;
	int count = 0;

	AreaLight(glm::vec3 p, glm::vec3 d, float h, float w, string name);
	AreaLight() {};
	~AreaLight() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor);
	void draw();

	glm::vec3 pointOnLight(int u, int v);
};