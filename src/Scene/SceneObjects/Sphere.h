#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

// General purpose sphere  (assume parametric)
class Sphere : public SceneObject {
public:
	float radius = 1.0;
	vector<glm::vec3> points;
	vector<glm::vec3> points2;
	vector<glm::vec3> normals;

	Sphere(glm::vec3 p, float r, string name, ofColor diffuse = ofColor::lightGray);
	Sphere() {}
	~Sphere() {};

	void setBounds();
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv);
	void draw();
	float sdf(glm::vec3 p);
	glm::vec2 getUV(glm::vec3 p);
};
