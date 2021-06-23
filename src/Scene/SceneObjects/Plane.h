#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

//  General purpose plane 
class Plane : public SceneObject {
public:
	ofPlanePrimitive plane;
	glm::vec3 normal;
	float width = 10;
	float height = 10;
	vector<glm::vec3> points;
	vector<glm::vec3> normals;
	glm::vec3 min, max;

	Plane(glm::vec3 p, glm::vec3 n, string name, ofColor diffuse = ofColor::darkOliveGreen, float w = 10, float h = 10);
	Plane();
	~Plane() {};

	void setBounds();
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor);
	void draw();
	float sdf(glm::vec3  p);

	glm::vec3 getNormal(const glm::vec3 &p);

	glm::vec2 getUV(glm::vec3 p);
};