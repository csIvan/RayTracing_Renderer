#pragma once

#include "ofMain.h"
#include "../Scene/SceneObject.h"

class BVH : public SceneObject {
public:

	BVH();
	~BVH() {};

	void create();
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv);
	void draw();
};