#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

//************************************ Cylinder Class ***************************************
class Cylinder : public SceneObject {
public:
	float height = 2.0f;
	float radius = 0.25f;	
	vector<glm::vec3> points;
	vector<glm::vec3> normals;
	int imageX, imageY;

	Cylinder(glm::vec3 p, float h, float r, string name, ofColor diffuse = ofColor::lightGray);
	Cylinder() {};
	~Cylinder() {};

	void setBounds();
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor);
	void draw();
	float sdf(const glm::vec3 p);
	glm::vec2 getUV(glm::vec3 p);

};