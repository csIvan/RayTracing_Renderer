#pragma once

#include "../SceneObject.h"

// 3D Cube scene object class
class Cube : public SceneObject {
private:
	float side = 1.0f;

public:
	Cube(const glm::vec3 &position, float side, const string &name, const ofColor &diffuse = DEFAULT_COLOR);
	~Cube() {};

	// Override functions from SceneObject
	void setBounds();
	void draw();
	bool intersect(const Ray &ray, HitInfo &hitInfo);
	float sdf(const glm::vec3 &point);
	glm::vec2 getUV(const glm::vec3 &point) const;


	// Setter and Getters
	void setSide(float side) { this->side = side; }
	float getSide() const { return side; }
};

