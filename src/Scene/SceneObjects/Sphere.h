#pragma once

#include "../SceneObject.h"

// 3D Sphere scene object class
class Sphere : public SceneObject {
private:
	float radius = 1.0f;

public:
	Sphere(const glm::vec3 &position, float radius, const string &name, const ofColor &diffuse = DEFAULT_COLOR);
	~Sphere() {};


	// Override functions from SceneObject
	void setBounds();
	void draw();
	bool intersect(const Ray &ray, HitInfo &hitInfo);
	float sdf(const glm::vec3 &point);
	glm::vec2 getUV(const glm::vec3 &point) const;


	// Setter and Getters
	void setRadius(float radius) { this->radius = radius; }
	float getRadius() const { return radius; }
};