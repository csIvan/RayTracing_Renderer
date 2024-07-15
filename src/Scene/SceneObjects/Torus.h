#pragma once

#include "../../Math/poly34.h"
#include "../SceneObject.h"

// 3D Torus scene object class
class Torus : public SceneObject {
private:
	float radius = 0.5f;
	float majorRadius = 1.0f;

	// OF draw strips
	int numc = 20;
	int numt = 35;

public:
	Torus(const glm::vec3 &position, float radius, float majorRadius, const string &name, const ofColor &diffuse = DEFAULT_COLOR);
	~Torus() {};


	// Override functions from SceneObject
	void setBounds();
	void draw();
	bool intersect(const Ray &ray, HitInfo &hitInfo);
	float sdf(const glm::vec3 &point);
	glm::vec2 getUV(const glm::vec3 &point) const;


	// Custom draw function for 3D torus
	void drawTorus();


	// Setter and Getters
	void setRadius(float radius) { this->radius = radius; }
	void setMajorRadius(float majorRadius) { this->majorRadius = majorRadius; }
	float getRadius() const { return radius; }
	float getMajorRadius() const { return majorRadius; }
};