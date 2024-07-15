#pragma once

#include "../SceneObject.h"

// 3D Plane scene object class
class Plane : public SceneObject {
private:
	ofPlanePrimitive plane;
	glm::vec3 normal;
	float width = 10.0f;
	float height = 10.0f;
	glm::vec3 min, max;

public:
	Plane(const glm::vec3 &position, const glm::vec3 &normal, const string &name, float width = 10.0f, float height = 10.0f, const ofColor &diffuse = DEFAULT_COLOR);
	~Plane() {};


	// Override functions from SceneObject
	void setBounds();
	void draw();
	bool intersect(const Ray &ray, HitInfo &hitInfo);
	float sdf(const glm::vec3 &point);
	glm::vec2 getUV(const glm::vec3 &point) const;


	// Setter and Getters
	void setWidth(float width) { this->width = width; }
	void setHeight(float height) { this->height = height; }
	float getWidth() const { return width; }
	float getHeight() const { return height; }
	glm::vec3 getNormal(const glm::vec3 &point) { return normal; }
};