#pragma once

#include "../SceneObject.h"

// 3D Cylinder scene object class
class Cylinder : public SceneObject {
private:
	float height = 2.0f;
	float radius = 0.25f;
public:
	Cylinder(const glm::vec3 &position, float height, float radius, const string &name, const ofColor &diffuse = DEFAULT_COLOR);
	~Cylinder() {};


	// Override functions from SceneObject
	void setBounds();
	void draw();
	bool intersect(const Ray &ray, HitInfo &hitInfo);
	float sdf(const glm::vec3 &point);
	glm::vec2 getUV(const glm::vec3 &point) const;


	// Setter and Getters
	void setRadius(float radius) { this->radius = radius; }
	void setHeight(float height) { this->height = height; }
	float getRadius() const { return radius; }
	float getHeight() const { return height; }
};